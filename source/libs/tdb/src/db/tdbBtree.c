/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tdbInt.h"

#define TDB_BTREE_ROOT 0x1
#define TDB_BTREE_LEAF 0x2

struct SBTree {
  SPgno          root;
  int            keyLen;
  int            valLen;
  SPager        *pPager;
  FKeyComparator kcmpr;
  int            pageSize;
  int            maxLocal;
  int            minLocal;
  int            maxLeaf;
  int            minLeaf;
  void          *pBuf;
};

#define TDB_BTREE_PAGE_COMMON_HDR u8 flags;

#define TDB_BTREE_PAGE_GET_FLAGS(PAGE)        (PAGE)->pData[0]
#define TDB_BTREE_PAGE_SET_FLAGS(PAGE, flags) ((PAGE)->pData[0] = (flags))
#define TDB_BTREE_PAGE_IS_ROOT(PAGE)          (TDB_BTREE_PAGE_GET_FLAGS(PAGE) & TDB_BTREE_ROOT)
#define TDB_BTREE_PAGE_IS_LEAF(PAGE)          (TDB_BTREE_PAGE_GET_FLAGS(PAGE) & TDB_BTREE_LEAF)
#define TDB_BTREE_ASSERT_FLAG(flags)                                                 \
  ASSERT(TDB_FLAG_IS(flags, TDB_BTREE_ROOT) || TDB_FLAG_IS(flags, TDB_BTREE_LEAF) || \
         TDB_FLAG_IS(flags, TDB_BTREE_ROOT | TDB_BTREE_LEAF) || TDB_FLAG_IS(flags, 0))

typedef struct __attribute__((__packed__)) {
  TDB_BTREE_PAGE_COMMON_HDR
} SLeafHdr;

typedef struct __attribute__((__packed__)) {
  TDB_BTREE_PAGE_COMMON_HDR;
  SPgno pgno;  // right-most child
} SIntHdr;

typedef struct {
  u8      flags;
  SBTree *pBt;
} SBtreeInitPageArg;

typedef struct {
  int       kLen;
  const u8 *pKey;
  int       vLen;
  const u8 *pVal;
  SPgno     pgno;
  u8       *pBuf;
} SCellDecoder;

static int tdbBtcMoveTo(SBTC *pBtc, const void *pKey, int kLen, int *pCRst);
static int tdbDefaultKeyCmprFn(const void *pKey1, int keyLen1, const void *pKey2, int keyLen2);
static int tdbBtreeOpenImpl(SBTree *pBt);
static int tdbBtreeZeroPage(SPage *pPage, void *arg);
static int tdbBtreeInitPage(SPage *pPage, void *arg);
static int tdbBtreeEncodeCell(SPage *pPage, const void *pKey, int kLen, const void *pVal, int vLen, SCell *pCell,
                              int *szCell);
static int tdbBtreeDecodeCell(SPage *pPage, const SCell *pCell, SCellDecoder *pDecoder);
static int tdbBtreeBalance(SBTC *pBtc);
static int tdbBtreeCellSize(const SPage *pPage, SCell *pCell);
static int tdbBtcMoveToNext(SBTC *pBtc);
static int tdbBtcMoveDownward(SBTC *pBtc);
static int tdbBtcMoveUpward(SBTC *pBtc);

int tdbBtreeOpen(int keyLen, int valLen, SPager *pPager, FKeyComparator kcmpr, SBTree **ppBt) {
  SBTree *pBt;
  int     ret;

  ASSERT(keyLen != 0);

  *ppBt = NULL;

  pBt = (SBTree *)tdbOsCalloc(1, sizeof(*pBt));
  if (pBt == NULL) {
    return -1;
  }

  // pBt->keyLen
  pBt->keyLen = keyLen < 0 ? TDB_VARIANT_LEN : keyLen;
  // pBt->valLen
  pBt->valLen = valLen < 0 ? TDB_VARIANT_LEN : valLen;
  // pBt->pPager
  pBt->pPager = pPager;
  // pBt->kcmpr
  pBt->kcmpr = kcmpr ? kcmpr : tdbDefaultKeyCmprFn;
  // pBt->pageSize
  pBt->pageSize = pPager->pageSize;
  // pBt->maxLocal
  pBt->maxLocal = tdbPageCapacity(pBt->pageSize, sizeof(SIntHdr)) / 4;
  // pBt->minLocal: Should not be allowed smaller than 15, which is [nPayload][nKey][nData]
  pBt->minLocal = pBt->maxLocal / 2;
  // pBt->maxLeaf
  pBt->maxLeaf = tdbPageCapacity(pBt->pageSize, sizeof(SLeafHdr));
  // pBt->minLeaf
  pBt->minLeaf = pBt->minLocal;

  // TODO: pBt->root
  ret = tdbBtreeOpenImpl(pBt);
  if (ret < 0) {
    tdbOsFree(pBt);
    return -1;
  }

  *ppBt = pBt;
  return 0;
}

int tdbBtreeClose(SBTree *pBt) {
  // TODO
  return 0;
}

int tdbBtreeInsert(SBTree *pBt, const void *pKey, int kLen, const void *pVal, int vLen) {
  SBTC   btc;
  SCell *pCell;
  void  *pBuf;
  int    szCell;
  int    szBuf;
  int    ret;
  int    idx;
  int    c;

  tdbBtcOpen(&btc, pBt);

  // move to the position to insert
  ret = tdbBtcMoveTo(&btc, pKey, kLen, &c);
  if (ret < 0) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }

  if (btc.idx == -1) {
    idx = 0;
  } else {
    if (c > 0) {
      idx = btc.idx + 1;
    } else if (c < 0) {
      idx = btc.idx;
    } else {
      // TDB does NOT allow same key
      tdbBtcClose(&btc);
      ASSERT(0);
      return -1;
    }
  }

  // make sure enough space to hold the cell
  szBuf = kLen + vLen + 14;
  pBuf = TDB_REALLOC(pBt->pBuf, pBt->pageSize > szBuf ? szBuf : pBt->pageSize);
  if (pBuf == NULL) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }
  pBt->pBuf = pBuf;
  pCell = (SCell *)pBt->pBuf;

  // encode cell
  ret = tdbBtreeEncodeCell(btc.pPage, pKey, kLen, pVal, vLen, pCell, &szCell);
  if (ret < 0) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }

  // mark the page dirty
  ret = tdbPagerWrite(pBt->pPager, btc.pPage);
  if (ret < 0) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }

  // insert the cell
  ret = tdbPageInsertCell(btc.pPage, idx, pCell, szCell, 0);
  if (ret < 0) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }

  // check if need balance
  if (btc.pPage->nOverflow > 0) {
    ret = tdbBtreeBalance(&btc);
    if (ret < 0) {
      tdbBtcClose(&btc);
      ASSERT(0);
      return -1;
    }
  }

  tdbBtcClose(&btc);

  return 0;
}

int tdbBtreeGet(SBTree *pBt, const void *pKey, int kLen, void **ppVal, int *vLen) {
  return tdbBtreePGet(pBt, pKey, kLen, NULL, NULL, ppVal, vLen);
}

int tdbBtreePGet(SBTree *pBt, const void *pKey, int kLen, void **ppKey, int *pkLen, void **ppVal, int *vLen) {
  SBTC         btc;
  SCell       *pCell;
  int          cret;
  int          ret;
  void        *pTKey = NULL;
  void        *pTVal = NULL;
  SCellDecoder cd;

  tdbBtcOpen(&btc, pBt);

  ret = tdbBtcMoveTo(&btc, pKey, kLen, &cret);
  if (ret < 0) {
    tdbBtcClose(&btc);
    ASSERT(0);
  }

  if (cret) {
    tdbBtcClose(&btc);
    return -1;
  }

  pCell = tdbPageGetCell(btc.pPage, btc.idx);
  tdbBtreeDecodeCell(btc.pPage, pCell, &cd);

  if (ppKey) {
    pTKey = TDB_REALLOC(*ppKey, cd.kLen);
    if (pTKey == NULL) {
      tdbBtcClose(&btc);
      ASSERT(0);
      return -1;
    }
    *ppKey = pTKey;
    *pkLen = cd.kLen;
    memcpy(*ppKey, cd.pKey, cd.kLen);
  }

  pTVal = TDB_REALLOC(*ppVal, cd.vLen);
  if (pTVal == NULL) {
    tdbBtcClose(&btc);
    ASSERT(0);
    return -1;
  }
  *ppVal = pTVal;
  *vLen = cd.vLen;
  memcpy(*ppVal, cd.pVal, cd.vLen);

  tdbBtcClose(&btc);

  return 0;
}

static int tdbDefaultKeyCmprFn(const void *pKey1, int keyLen1, const void *pKey2, int keyLen2) {
  int mlen;
  int cret;

  ASSERT(keyLen1 > 0 && keyLen2 > 0 && pKey1 != NULL && pKey2 != NULL);

  mlen = keyLen1 < keyLen2 ? keyLen1 : keyLen2;
  cret = memcmp(pKey1, pKey2, mlen);
  if (cret == 0) {
    if (keyLen1 < keyLen2) {
      cret = -1;
    } else if (keyLen1 > keyLen2) {
      cret = 1;
    } else {
      cret = 0;
    }
  }
  return cret;
}

static int tdbBtreeOpenImpl(SBTree *pBt) {
  // Try to get the root page of the an existing btree

  SPgno  pgno;
  SPage *pPage;
  int    ret;

  {
    // 1. TODO: Search the main DB to check if the DB exists
    pgno = 0;
  }

  if (pgno != 0) {
    pBt->root = pgno;
    return 0;
  }

  // Try to create a new database
  SBtreeInitPageArg zArg = {.flags = TDB_BTREE_ROOT | TDB_BTREE_LEAF, .pBt = pBt};
  ret = tdbPagerNewPage(pBt->pPager, &pgno, &pPage, tdbBtreeZeroPage, &zArg);
  if (ret < 0) {
    return -1;
  }

  // TODO: Unref the page

  ASSERT(pgno != 0);
  pBt->root = pgno;

  return 0;
}

static int tdbBtreeInitPage(SPage *pPage, void *arg) {
  SBTree *pBt;
  u8      flags;
  u8      isLeaf;

  pBt = (SBTree *)arg;
  flags = TDB_BTREE_PAGE_GET_FLAGS(pPage);
  isLeaf = TDB_BTREE_PAGE_IS_LEAF(pPage);

  ASSERT(flags == TDB_BTREE_PAGE_GET_FLAGS(pPage));

  tdbPageInit(pPage, isLeaf ? sizeof(SLeafHdr) : sizeof(SIntHdr), tdbBtreeCellSize);

  TDB_BTREE_ASSERT_FLAG(flags);

  if (isLeaf) {
    pPage->kLen = pBt->keyLen;
    pPage->vLen = pBt->valLen;
    pPage->maxLocal = pBt->maxLeaf;
    pPage->minLocal = pBt->minLeaf;
  } else {
    pPage->kLen = pBt->keyLen;
    pPage->vLen = sizeof(SPgno);
    pPage->maxLocal = pBt->maxLocal;
    pPage->minLocal = pBt->minLocal;
  }

  return 0;
}

static int tdbBtreeZeroPage(SPage *pPage, void *arg) {
  u8      flags;
  SBTree *pBt;
  u8      leaf;

  flags = ((SBtreeInitPageArg *)arg)->flags;
  pBt = ((SBtreeInitPageArg *)arg)->pBt;
  leaf = flags & TDB_BTREE_LEAF;

  tdbPageZero(pPage, leaf ? sizeof(SLeafHdr) : sizeof(SIntHdr), tdbBtreeCellSize);

  if (leaf) {
    SLeafHdr *pLeafHdr = (SLeafHdr *)(pPage->pData);
    pLeafHdr->flags = flags;

    pPage->kLen = pBt->keyLen;
    pPage->vLen = pBt->valLen;
    pPage->maxLocal = pBt->maxLeaf;
    pPage->minLocal = pBt->minLeaf;
  } else {
    SIntHdr *pIntHdr = (SIntHdr *)(pPage->pData);
    pIntHdr->flags = flags;
    pIntHdr->pgno = 0;

    pPage->kLen = pBt->keyLen;
    pPage->vLen = sizeof(SPgno);
    pPage->maxLocal = pBt->maxLocal;
    pPage->minLocal = pBt->minLocal;
  }

  return 0;
}

#ifndef TDB_BTREE_BALANCE
typedef struct {
  SBTree *pBt;
  SPage  *pParent;
  int     idx;
  i8      nOld;
  SPage  *pOldPages[3];
  i8      nNewPages;
  SPage  *pNewPages[5];
} SBtreeBalanceHelper;

static int tdbBtreeBalanceDeeper(SBTree *pBt, SPage *pRoot, SPage **ppChild) {
  SPager           *pPager;
  SPage            *pChild;
  SPgno             pgnoChild;
  int               ret;
  u8                flags;
  SIntHdr          *pIntHdr;
  SBtreeInitPageArg zArg;
  u8                leaf;

  pPager = pRoot->pPager;
  flags = TDB_BTREE_PAGE_GET_FLAGS(pRoot);
  leaf = TDB_BTREE_PAGE_IS_LEAF(pRoot);

  // Allocate a new child page
  zArg.flags = TDB_FLAG_REMOVE(flags, TDB_BTREE_ROOT);
  zArg.pBt = pBt;
  ret = tdbPagerNewPage(pPager, &pgnoChild, &pChild, tdbBtreeZeroPage, &zArg);
  if (ret < 0) {
    return -1;
  }

  if (!leaf) {
    ((SIntHdr *)pChild->pData)->pgno = ((SIntHdr *)(pRoot->pData))->pgno;
  }

  // Copy the root page content to the child page
  tdbPageCopy(pRoot, pChild);

  // Reinitialize the root page
  zArg.flags = TDB_BTREE_ROOT;
  zArg.pBt = pBt;
  ret = tdbBtreeZeroPage(pRoot, &zArg);
  if (ret < 0) {
    return -1;
  }

  pIntHdr = (SIntHdr *)(pRoot->pData);
  pIntHdr->pgno = pgnoChild;

  *ppChild = pChild;
  return 0;
}

static int tdbBtreeBalanceNonRoot(SBTree *pBt, SPage *pParent, int idx) {
  int ret;

  int    nOlds;
  SPage *pOlds[3] = {0};
  SCell *pDivCell[3] = {0};
  int    szDivCell[3];
  int    sIdx;
  u8     childNotLeaf;
  SPgno  rPgno;

  {  // Find 3 child pages at most to do balance
    int    nCells = TDB_PAGE_TOTAL_CELLS(pParent);
    SCell *pCell;

    if (nCells <= 2) {
      sIdx = 0;
      nOlds = nCells + 1;
    } else {
      // has more than three child pages
      if (idx == 0) {
        sIdx = 0;
      } else if (idx == nCells) {
        sIdx = idx - 2;
      } else {
        sIdx = idx - 1;
      }
      nOlds = 3;
    }
    for (int i = 0; i < nOlds; i++) {
      ASSERT(sIdx + i <= nCells);

      SPgno pgno;
      if (sIdx + i == nCells) {
        ASSERT(!TDB_BTREE_PAGE_IS_LEAF(pParent));
        pgno = ((SIntHdr *)(pParent->pData))->pgno;
      } else {
        pCell = tdbPageGetCell(pParent, sIdx + i);
        pgno = *(SPgno *)pCell;
      }

      ret = tdbPagerFetchPage(pBt->pPager, pgno, pOlds + i, tdbBtreeInitPage, pBt);
      if (ret < 0) {
        ASSERT(0);
        return -1;
      }
    }
    // copy the parent key out if child pages are not leaf page
    childNotLeaf = !TDB_BTREE_PAGE_IS_LEAF(pOlds[0]);
    if (childNotLeaf) {
      for (int i = 0; i < nOlds; i++) {
        if (sIdx + i < TDB_PAGE_TOTAL_CELLS(pParent)) {
          pCell = tdbPageGetCell(pParent, sIdx + i);
          szDivCell[i] = tdbBtreeCellSize(pParent, pCell);
          pDivCell[i] = tdbOsMalloc(szDivCell[i]);
          memcpy(pDivCell[i], pCell, szDivCell[i]);
        }

        if (i < nOlds - 1) {
          ((SPgno *)pDivCell[i])[0] = ((SIntHdr *)pOlds[i]->pData)->pgno;
          ((SIntHdr *)pOlds[i]->pData)->pgno = 0;
          tdbPageInsertCell(pOlds[i], TDB_PAGE_TOTAL_CELLS(pOlds[i]), pDivCell[i], szDivCell[i], 1);
        }
      }
      rPgno = ((SIntHdr *)pOlds[nOlds - 1]->pData)->pgno;
    }
    // drop the cells on parent page
    for (int i = 0; i < nOlds; i++) {
      nCells = TDB_PAGE_TOTAL_CELLS(pParent);
      if (sIdx < nCells) {
        tdbPageDropCell(pParent, sIdx);
      } else {
        ((SIntHdr *)pParent->pData)->pgno = 0;
      }
    }
  }

  int nNews = 0;
  struct {
    int cnt;
    int size;
    int iPage;
    int oIdx;
  } infoNews[5] = {0};

  {  // Get how many new pages are needed and the new distribution

    // first loop to find minimum number of pages needed
    for (int oPage = 0; oPage < nOlds; oPage++) {
      SPage *pPage = pOlds[oPage];
      SCell *pCell;
      int    cellBytes;
      int    oIdx;

      for (oIdx = 0; oIdx < TDB_PAGE_TOTAL_CELLS(pPage); oIdx++) {
        pCell = tdbPageGetCell(pPage, oIdx);
        cellBytes = TDB_BYTES_CELL_TAKEN(pPage, pCell);

        if (infoNews[nNews].size + cellBytes > TDB_PAGE_USABLE_SIZE(pPage)) {
          // page is full, use a new page
          nNews++;

          ASSERT(infoNews[nNews].size + cellBytes <= TDB_PAGE_USABLE_SIZE(pPage));

          if (childNotLeaf) {
            // for non-child page, this cell is used as the right-most child,
            // the divider cell to parent as well
            continue;
          }
        }
        infoNews[nNews].cnt++;
        infoNews[nNews].size += cellBytes;
        infoNews[nNews].iPage = oPage;
        infoNews[nNews].oIdx = oIdx;
      }
    }

    nNews++;

    // back loop to make the distribution even
    for (int iNew = nNews - 1; iNew > 0; iNew--) {
      SCell *pCell;
      int    szLCell, szRCell;

      for (;;) {
        pCell = tdbPageGetCell(pOlds[infoNews[iNew - 1].iPage], infoNews[iNew - 1].oIdx);

        if (childNotLeaf) {
          szLCell = szRCell = tdbBtreeCellSize(pOlds[infoNews[iNew - 1].iPage], pCell);
        } else {
          szLCell = tdbBtreeCellSize(pOlds[infoNews[iNew - 1].iPage], pCell);

          int    iPage = infoNews[iNew - 1].iPage;
          int    oIdx = infoNews[iNew - 1].oIdx + 1;
          SPage *pPage;
          for (;;) {
            pPage = pOlds[iPage];
            if (oIdx < TDB_PAGE_TOTAL_CELLS(pPage)) {
              break;
            }

            iPage++;
            oIdx = 0;
          }

          pCell = tdbPageGetCell(pPage, oIdx);
          szRCell = tdbBtreeCellSize(pPage, pCell);
        }

        ASSERT(infoNews[iNew - 1].cnt > 0);

        if (infoNews[iNew].size + szRCell >= infoNews[iNew - 1].size - szRCell) {
          break;
        }

        // Move a cell right forward
        infoNews[iNew - 1].cnt--;
        infoNews[iNew - 1].size -= szLCell;
        infoNews[iNew - 1].oIdx--;
        for (;;) {
          if (infoNews[iNew - 1].oIdx >= 0) {
            break;
          }

          infoNews[iNew - 1].iPage--;
          infoNews[iNew - 1].oIdx = TDB_PAGE_TOTAL_CELLS(pOlds[infoNews[iNew - 1].iPage]) - 1;
        }

        infoNews[iNew].cnt++;
        infoNews[iNew].size += szRCell;
      }
    }
  }

  SPage *pNews[5] = {0};
  {  // Allocate new pages, reuse the old page when possible

    SPgno             pgno;
    SBtreeInitPageArg iarg;
    u8                flags;

    flags = TDB_BTREE_PAGE_GET_FLAGS(pOlds[0]);

    for (int iNew = 0; iNew < nNews; iNew++) {
      if (iNew < nOlds) {
        pNews[iNew] = pOlds[iNew];
      } else {
        iarg.pBt = pBt;
        iarg.flags = flags;
        ret = tdbPagerNewPage(pBt->pPager, &pgno, pNews + iNew, tdbBtreeZeroPage, &iarg);
        if (ret < 0) {
          ASSERT(0);
        }
      }
    }

    // TODO: sort the page according to the page number
  }

  {  // Do the real cell distribution
    SPage            *pOldsCopy[3] = {0};
    SCell            *pCell;
    int               szCell;
    SBtreeInitPageArg iarg;
    int               iNew, nNewCells;
    SCellDecoder      cd;

    iarg.pBt = pBt;
    iarg.flags = TDB_BTREE_PAGE_GET_FLAGS(pOlds[0]);
    for (int i = 0; i < nOlds; i++) {
      tdbPageCreate(pOlds[0]->pageSize, &pOldsCopy[i], NULL, NULL);
      tdbBtreeZeroPage(pOldsCopy[i], &iarg);
      tdbPageCopy(pOlds[i], pOldsCopy[i]);
    }
    iNew = 0;
    nNewCells = 0;
    tdbBtreeZeroPage(pNews[iNew], &iarg);

    for (int iOld = 0; iOld < nOlds; iOld++) {
      SPage *pPage;

      pPage = pOldsCopy[iOld];

      for (int oIdx = 0; oIdx < TDB_PAGE_TOTAL_CELLS(pPage); oIdx++) {
        pCell = tdbPageGetCell(pPage, oIdx);
        szCell = tdbBtreeCellSize(pPage, pCell);

        ASSERT(nNewCells <= infoNews[iNew].cnt);
        ASSERT(iNew < nNews);

        if (nNewCells < infoNews[iNew].cnt) {
          tdbPageInsertCell(pNews[iNew], nNewCells, pCell, szCell, 0);
          nNewCells++;

          // insert parent page
          if (!childNotLeaf && nNewCells == infoNews[iNew].cnt) {
            SIntHdr *pIntHdr = (SIntHdr *)pParent->pData;

            if (iNew == nNews - 1 && pIntHdr->pgno == 0) {
              pIntHdr->pgno = TDB_PAGE_PGNO(pNews[iNew]);
            } else {
              tdbBtreeDecodeCell(pPage, pCell, &cd);

              // TODO: pCell here may be inserted as an overflow cell, handle it
              SCell *pNewCell = tdbOsMalloc(cd.kLen + 9);
              int    szNewCell;
              SPgno  pgno;
              pgno = TDB_PAGE_PGNO(pNews[iNew]);
              tdbBtreeEncodeCell(pParent, cd.pKey, cd.kLen, (void *)&pgno, sizeof(SPgno), pNewCell, &szNewCell);
              tdbPageInsertCell(pParent, sIdx++, pNewCell, szNewCell, 0);
              tdbOsFree(pNewCell);
            }

            // move to next new page
            iNew++;
            nNewCells = 0;
            if (iNew < nNews) {
              tdbBtreeZeroPage(pNews[iNew], &iarg);
            }
          }
        } else {
          ASSERT(childNotLeaf);
          ASSERT(iNew < nNews - 1);

          // set current new page right-most child
          ((SIntHdr *)pNews[iNew]->pData)->pgno = ((SPgno *)pCell)[0];

          // insert to parent as divider cell
          ASSERT(iNew < nNews - 1);
          ((SPgno *)pCell)[0] = TDB_PAGE_PGNO(pNews[iNew]);
          tdbPageInsertCell(pParent, sIdx++, pCell, szCell, 0);

          // move to next new page
          iNew++;
          nNewCells = 0;
          if (iNew < nNews) {
            tdbBtreeZeroPage(pNews[iNew], &iarg);
          }
        }
      }
    }

    if (childNotLeaf) {
      ASSERT(TDB_PAGE_TOTAL_CELLS(pNews[nNews - 1]) == infoNews[nNews - 1].cnt);
      ((SIntHdr *)(pNews[nNews - 1]->pData))->pgno = rPgno;

      SIntHdr *pIntHdr = (SIntHdr *)pParent->pData;
      if (pIntHdr->pgno == 0) {
        pIntHdr->pgno = TDB_PAGE_PGNO(pNews[nNews - 1]);
      } else {
        ((SPgno *)pDivCell[nOlds - 1])[0] = TDB_PAGE_PGNO(pNews[nNews - 1]);
        tdbPageInsertCell(pParent, sIdx, pDivCell[nOlds - 1], szDivCell[nOlds - 1], 0);
      }
    }

    for (int i = 0; i < nOlds; i++) {
      tdbPageDestroy(pOldsCopy[i], NULL, NULL);
    }
  }

  for (int i = 0; i < 3; i++) {
    if (pDivCell[i]) {
      tdbOsFree(pDivCell[i]);
    }
  }

  return 0;
}

static int tdbBtreeBalance(SBTC *pBtc) {
  int    iPage;
  SPage *pParent;
  SPage *pPage;
  int    ret;
  u8     flags;
  u8     leaf;
  u8     root;

  // Main loop to balance the BTree
  for (;;) {
    iPage = pBtc->iPage;
    pPage = pBtc->pPage;
    leaf = TDB_BTREE_PAGE_IS_LEAF(pPage);
    root = TDB_BTREE_PAGE_IS_ROOT(pPage);

    // when the page is not overflow and not too empty, the balance work
    // is finished. Just break out the balance loop.
    if (pPage->nOverflow == 0 /* TODO: && pPage->nFree <= */) {
      break;
    }

    if (iPage == 0) {
      // For the root page, only balance when the page is overfull,
      // ignore the case of empty
      if (pPage->nOverflow == 0) break;

      ret = tdbBtreeBalanceDeeper(pBtc->pBt, pPage, &(pBtc->pgStack[1]));
      if (ret < 0) {
        return -1;
      }

      pBtc->idx = 0;
      pBtc->idxStack[0] = 0;
      pBtc->pgStack[0] = pBtc->pPage;
      pBtc->iPage = 1;
      pBtc->pPage = pBtc->pgStack[1];
    } else {
      // Generalized balance step
      pParent = pBtc->pgStack[iPage - 1];

      ret = tdbBtreeBalanceNonRoot(pBtc->pBt, pParent, pBtc->idxStack[pBtc->iPage - 1]);
      if (ret < 0) {
        return -1;
      }

      pBtc->iPage--;
      pBtc->pPage = pBtc->pgStack[pBtc->iPage];
    }
  }

  return 0;
}
#endif

// TDB_BTREE_CELL =====================
static int tdbBtreeEncodePayload(SPage *pPage, SCell *pCell, int nHeader, const void *pKey, int kLen, const void *pVal,
                                 int vLen, int *szPayload) {
  int nPayload;

  nPayload = kLen + vLen;
  if (nPayload + nHeader <= pPage->maxLocal) {
    // no overflow page is needed
    memcpy(pCell + nHeader, pKey, kLen);
    if (pVal) {
      memcpy(pCell + nHeader + kLen, pVal, vLen);
    }

    *szPayload = nPayload;
    return 0;
  }

  {
    // TODO: handle overflow case
    ASSERT(0);
  }

  return 0;
}

static int tdbBtreeEncodeCell(SPage *pPage, const void *pKey, int kLen, const void *pVal, int vLen, SCell *pCell,
                              int *szCell) {
  u8  leaf;
  int nHeader;
  int nPayload;
  int ret;

  ASSERT(pPage->kLen == TDB_VARIANT_LEN || pPage->kLen == kLen);
  ASSERT(pPage->vLen == TDB_VARIANT_LEN || pPage->vLen == vLen);
  ASSERT(pKey != NULL && kLen > 0);

  nPayload = 0;
  nHeader = 0;
  leaf = TDB_BTREE_PAGE_IS_LEAF(pPage);

  // 1. Encode Header part
  /* Encode SPgno if interior page */
  if (!leaf) {
    ASSERT(pPage->vLen == sizeof(SPgno));

    ((SPgno *)(pCell + nHeader))[0] = ((SPgno *)pVal)[0];
    nHeader = nHeader + sizeof(SPgno);
  }

  /* Encode kLen if need */
  if (pPage->kLen == TDB_VARIANT_LEN) {
    nHeader += tdbPutVarInt(pCell + nHeader, kLen);
  }

  /* Encode vLen if need */
  if (pPage->vLen == TDB_VARIANT_LEN) {
    nHeader += tdbPutVarInt(pCell + nHeader, vLen);
  }

  // 2. Encode payload part
  if ((!leaf) || pPage->vLen == 0) {
    pVal = NULL;
    vLen = 0;
  }

  ret = tdbBtreeEncodePayload(pPage, pCell, nHeader, pKey, kLen, pVal, vLen, &nPayload);
  if (ret < 0) {
    // TODO
    ASSERT(0);
    return 0;
  }

  *szCell = nHeader + nPayload;
  return 0;
}

static int tdbBtreeDecodePayload(SPage *pPage, const SCell *pCell, int nHeader, SCellDecoder *pDecoder) {
  int nPayload;

  if (pDecoder->pVal) {
    ASSERT(!TDB_BTREE_PAGE_IS_LEAF(pPage));
    nPayload = pDecoder->kLen;
  } else {
    nPayload = pDecoder->kLen + pDecoder->vLen;
  }

  if (nHeader + nPayload <= pPage->maxLocal) {
    // no over flow case
    pDecoder->pKey = pCell + nHeader;
    if (pDecoder->pVal == NULL && pDecoder->vLen > 0) {
      pDecoder->pVal = pCell + nHeader + pDecoder->kLen;
    }
    return 0;
  }

  {
    // TODO: handle overflow case
    ASSERT(0);
  }

  return 0;
}

static int tdbBtreeDecodeCell(SPage *pPage, const SCell *pCell, SCellDecoder *pDecoder) {
  u8  leaf;
  int nHeader;
  int ret;

  nHeader = 0;
  leaf = TDB_BTREE_PAGE_IS_LEAF(pPage);

  // Clear the state of decoder
  pDecoder->kLen = -1;
  pDecoder->pKey = NULL;
  pDecoder->vLen = -1;
  pDecoder->pVal = NULL;
  pDecoder->pgno = 0;

  // 1. Decode header part
  if (!leaf) {
    ASSERT(pPage->vLen == sizeof(SPgno));

    pDecoder->pgno = ((SPgno *)(pCell + nHeader))[0];
    pDecoder->pVal = (u8 *)(&(pDecoder->pgno));
    nHeader = nHeader + sizeof(SPgno);
  }

  if (pPage->kLen == TDB_VARIANT_LEN) {
    nHeader += tdbGetVarInt(pCell + nHeader, &(pDecoder->kLen));
  } else {
    pDecoder->kLen = pPage->kLen;
  }

  if (pPage->vLen == TDB_VARIANT_LEN) {
    ASSERT(leaf);
    nHeader += tdbGetVarInt(pCell + nHeader, &(pDecoder->vLen));
  } else {
    pDecoder->vLen = pPage->vLen;
  }

  // 2. Decode payload part
  ret = tdbBtreeDecodePayload(pPage, pCell, nHeader, pDecoder);
  if (ret < 0) {
    return -1;
  }

  return 0;
}

static int tdbBtreeCellSize(const SPage *pPage, SCell *pCell) {
  u8  leaf;
  int szCell;
  int kLen = 0, vLen = 0;

  leaf = TDB_BTREE_PAGE_IS_LEAF(pPage);
  szCell = 0;

  if (!leaf) {
    szCell += sizeof(SPgno);
  }

  if (pPage->kLen == TDB_VARIANT_LEN) {
    szCell += tdbGetVarInt(pCell + szCell, &kLen);
  } else {
    kLen = pPage->kLen;
  }

  if (pPage->vLen == TDB_VARIANT_LEN) {
    ASSERT(leaf);
    szCell += tdbGetVarInt(pCell + szCell, &vLen);
  } else if (leaf) {
    vLen = pPage->vLen;
  }

  szCell = szCell + kLen + vLen;

  if (szCell <= pPage->maxLocal) {
    return szCell;
  }

  {
    // TODO
    ASSERT(0);
    return 0;
  }
}
// TDB_BTREE_CELL

// TDB_BTREE_CURSOR =====================
int tdbBtcOpen(SBTC *pBtc, SBTree *pBt) {
  pBtc->pBt = pBt;
  pBtc->iPage = -1;
  pBtc->pPage = NULL;
  pBtc->idx = -1;

  return 0;
}

int tdbBtcMoveToFirst(SBTC *pBtc) {
  int     ret;
  SBTree *pBt;
  SPager *pPager;
  SCell  *pCell;
  SPgno   pgno;

  pBt = pBtc->pBt;
  pPager = pBt->pPager;

  if (pBtc->iPage < 0) {
    // move a clean cursor
    ret = tdbPagerFetchPage(pPager, pBt->root, &(pBtc->pPage), tdbBtreeInitPage, pBt);
    if (ret < 0) {
      ASSERT(0);
      return -1;
    }

    ASSERT(TDB_BTREE_PAGE_IS_ROOT(pBtc->pPage));

    pBtc->iPage = 0;
    if (TDB_PAGE_TOTAL_CELLS(pBtc->pPage) > 0) {
      pBtc->idx = 0;
    } else {
      // no any data, point to an invalid position
      ASSERT(TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage));
      pBtc->idx = -1;
      return 0;
    }
  } else {
    // move from a position
    int iPage = 0;

    for (; iPage < pBtc->iPage; iPage++) {
      ASSERT(pBtc->idxStack[iPage] >= 0);
      if (pBtc->idxStack[iPage]) break;
    }

    // move upward
    for (;;) {
      if (pBtc->iPage == 0) {
        pBtc->idx = 0;
        break;
      }

      if (pBtc->iPage < iPage) break;
      tdbBtcMoveUpward(pBtc);
    }
  }

  // move downward
  for (;;) {
    if (TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage)) break;

    ret = tdbBtcMoveDownward(pBtc);
    if (ret < 0) {
      ASSERT(0);
      return -1;
    }

    pBtc->idx = 0;
  }

  return 0;
}

int tdbBtcMoveToLast(SBTC *pBtc) {
  int     ret;
  int     nCells;
  SBTree *pBt;
  SPager *pPager;
  SPgno   pgno;

  pBt = pBtc->pBt;
  pPager = pBt->pPager;

  if (pBtc->iPage < 0) {
    // move a clean cursor
    ret = tdbPagerFetchPage(pPager, pBt->root, &(pBtc->pPage), tdbBtreeInitPage, pBt);
    if (ret < 0) {
      ASSERT(0);
      return -1;
    }

    nCells = TDB_PAGE_TOTAL_CELLS(pBtc->pPage);
    pBtc->iPage = 0;
    if (nCells > 0) {
      pBtc->idx = TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage) ? nCells - 1 : nCells;
    } else {
      // no data at all, point to an invalid position
      ASSERT(TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage));
      pBtc->idx = -1;
      return 0;
    }
  } else {
    int iPage = 0;

    // downward search
    for (; iPage < pBtc->iPage; iPage++) {
      ASSERT(!TDB_BTREE_PAGE_IS_LEAF(pBtc->pgStack[iPage]));
      nCells = TDB_PAGE_TOTAL_CELLS(pBtc->pgStack[iPage]);
      if (pBtc->idxStack[iPage] != nCells) break;
    }

    // move upward
    for (;;) {
      if (pBtc->iPage == 0) {
        if (TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage)) {
          pBtc->idx = TDB_PAGE_TOTAL_CELLS(pBtc->pPage) - 1;
        } else {
          pBtc->idx = TDB_PAGE_TOTAL_CELLS(pBtc->pPage);
        }
      }

      if (pBtc->iPage < iPage) break;
      tdbBtcMoveUpward(pBtc);
    }
  }

  // move downward
  for (;;) {
    if (TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage)) break;

    ret = tdbBtcMoveDownward(pBtc);
    if (ret < 0) {
      ASSERT(0);
      return -1;
    }

    nCells = TDB_PAGE_TOTAL_CELLS(pBtc->pPage);
    if (TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage)) {
      pBtc->idx = nCells - 1;
    } else {
      pBtc->idx = nCells;
    }
  }

  return 0;
}

int tdbBtreeNext(SBTC *pBtc, void **ppKey, int *kLen, void **ppVal, int *vLen) {
  SCell       *pCell;
  SCellDecoder cd;
  void        *pKey, *pVal;
  int          ret;

  // current cursor points to an invalid position
  if (pBtc->idx < 0) {
    return -1;
  }

  pCell = tdbPageGetCell(pBtc->pPage, pBtc->idx);

  tdbBtreeDecodeCell(pBtc->pPage, pCell, &cd);

  pKey = TDB_REALLOC(*ppKey, cd.kLen);
  if (pKey == NULL) {
    return -1;
  }

  // TODO: vLen may be zero
  pVal = TDB_REALLOC(*ppVal, cd.vLen);
  if (pVal == NULL) {
    TDB_FREE(pKey);
    return -1;
  }

  *ppKey = pKey;
  *ppVal = pVal;

  *kLen = cd.kLen;
  *vLen = cd.vLen;

  memcpy(pKey, cd.pKey, cd.kLen);
  memcpy(pVal, cd.pVal, cd.vLen);

  ret = tdbBtcMoveToNext(pBtc);
  if (ret < 0) {
    ASSERT(0);
    return -1;
  }

  return 0;
}

static int tdbBtcMoveToNext(SBTC *pBtc) {
  int    nCells;
  int    ret;
  SCell *pCell;

  ASSERT(TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage));

  if (pBtc->idx < 0) return -1;

  pBtc->idx++;
  if (pBtc->idx < TDB_PAGE_TOTAL_CELLS(pBtc->pPage)) {
    return 0;
  }

  // move upward
  for (;;) {
    if (pBtc->iPage == 0) {
      pBtc->idx = -1;
      return 0;
    }

    tdbBtcMoveUpward(pBtc);
    pBtc->idx++;

    ASSERT(!TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage));
    if (pBtc->idx <= TDB_PAGE_TOTAL_CELLS(pBtc->pPage)) {
      break;
    }
  }

  // move downward
  for (;;) {
    if (TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage)) break;

    ret = tdbBtcMoveDownward(pBtc);
    if (ret < 0) {
      ASSERT(0);
      return -1;
    }

    pBtc->idx = 0;
  }

  return 0;
}

static int tdbBtcMoveDownward(SBTC *pBtc) {
  int    ret;
  SPgno  pgno;
  SCell *pCell;

  ASSERT(pBtc->idx >= 0);
  ASSERT(!TDB_BTREE_PAGE_IS_LEAF(pBtc->pPage));

  if (pBtc->idx < TDB_PAGE_TOTAL_CELLS(pBtc->pPage)) {
    pCell = tdbPageGetCell(pBtc->pPage, pBtc->idx);
    pgno = ((SPgno *)pCell)[0];
  } else {
    pgno = ((SIntHdr *)pBtc->pPage->pData)->pgno;
  }

  pBtc->pgStack[pBtc->iPage] = pBtc->pPage;
  pBtc->idxStack[pBtc->iPage] = pBtc->idx;
  pBtc->iPage++;
  pBtc->pPage = NULL;
  pBtc->idx = -1;

  ret = tdbPagerFetchPage(pBtc->pBt->pPager, pgno, &pBtc->pPage, tdbBtreeInitPage, pBtc->pBt);
  if (ret < 0) {
    ASSERT(0);
    return -1;
  }

  return 0;
}

static int tdbBtcMoveUpward(SBTC *pBtc) {
  if (pBtc->iPage == 0) return -1;

  tdbPagerReturnPage(pBtc->pBt->pPager, pBtc->pPage);

  pBtc->iPage--;
  pBtc->pPage = pBtc->pgStack[pBtc->iPage];
  pBtc->idx = pBtc->idxStack[pBtc->iPage];

  return 0;
}

static int tdbBtcMoveTo(SBTC *pBtc, const void *pKey, int kLen, int *pCRst) {
  int          ret;
  SBTree      *pBt;
  SCell       *pCell;
  SPager      *pPager;
  SCellDecoder cd = {0};

  pBt = pBtc->pBt;
  pPager = pBt->pPager;

  if (pBtc->iPage < 0) {
    // move from a clear cursor
    ret = tdbPagerFetchPage(pPager, pBt->root, &(pBtc->pPage), tdbBtreeInitPage, pBt);
    if (ret < 0) {
      // TODO
      ASSERT(0);
      return 0;
    }

    pBtc->iPage = 0;
    pBtc->idx = -1;
    // for empty tree, just return with an invalid position
    if (TDB_PAGE_TOTAL_CELLS(pBtc->pPage) == 0) return 0;
  } else {
    // move upward to a page that the search key is in the range
    ASSERT(0);
  }

  // search downward to the leaf
  for (;;) {
    int    lidx, ridx, midx, c, nCells;
    SPage *pPage;

    pPage = pBtc->pPage;
    nCells = TDB_PAGE_TOTAL_CELLS(pPage);
    lidx = 0;
    ridx = nCells - 1;

    ASSERT(nCells > 0);
    ASSERT(pBtc->idx == -1);

    // binary search
    for (;;) {
      if (lidx > ridx) break;

      midx = (lidx + ridx) >> 1;

      pCell = tdbPageGetCell(pPage, midx);
      ret = tdbBtreeDecodeCell(pPage, pCell, &cd);
      if (ret < 0) {
        // TODO: handle error
        ASSERT(0);
        return -1;
      }

      // Compare the key values
      c = pBt->kcmpr(pKey, kLen, cd.pKey, cd.kLen);
      if (c < 0) {
        // pKey < cd.pKey
        ridx = midx - 1;
      } else if (c > 0) {
        // pKey > cd.pKey
        lidx = midx + 1;
      } else {
        // pKey == cd.pKey
        break;
      }
    }

    // keep search downward or break
    if (TDB_BTREE_PAGE_IS_LEAF(pPage)) {
      pBtc->idx = midx;
      *pCRst = c;
      break;
    } else {
      if (c <= 0) {
        pBtc->idx = midx;
      } else {
        pBtc->idx = midx + 1;
      }
      tdbBtcMoveDownward(pBtc);
    }
  }

  return 0;
}

int tdbBtcClose(SBTC *pBtc) {
  if (pBtc->iPage < 0) return 0;

  for (;;) {
    ASSERT(pBtc->pPage);

    tdbPagerReturnPage(pBtc->pBt->pPager, pBtc->pPage);

    pBtc->iPage--;
    if (pBtc->iPage < 0) break;

    pBtc->pPage = pBtc->pgStack[pBtc->iPage];
    pBtc->idx = pBtc->idxStack[pBtc->iPage];
  }

  return 0;
}
// TDB_BTREE_CURSOR

// TDB_BTREE_DEBUG =====================
#ifndef NODEBUG
typedef struct {
  SPgno pgno;
  u8    root;
  u8    leaf;
  SPgno rChild;
  int   nCells;
  int   nOvfl;
} SBtPageInfo;

SBtPageInfo btPageInfos[20];

void tdbBtPageInfo(SPage *pPage, int idx) {
  SBtPageInfo *pBtPageInfo;

  pBtPageInfo = btPageInfos + idx;

  pBtPageInfo->pgno = TDB_PAGE_PGNO(pPage);

  pBtPageInfo->root = TDB_BTREE_PAGE_IS_ROOT(pPage);
  pBtPageInfo->leaf = TDB_BTREE_PAGE_IS_LEAF(pPage);

  pBtPageInfo->rChild = 0;
  if (!pBtPageInfo->leaf) {
    pBtPageInfo->rChild = *(SPgno *)(pPage->pData + 1);
  }

  pBtPageInfo->nCells = TDB_PAGE_TOTAL_CELLS(pPage) - pPage->nOverflow;
  pBtPageInfo->nOvfl = pPage->nOverflow;
}
#endif
// TDB_BTREE_DEBUG