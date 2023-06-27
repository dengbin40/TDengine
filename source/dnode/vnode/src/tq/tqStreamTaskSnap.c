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

#include "meta.h"
#include "tdbInt.h"
#include "tq.h"

// STqSnapReader ========================================
struct SStreamTaskReader {
  STQ*    pTq;
  int64_t sver;
  int64_t ever;
  TBC*    pCur;
};

int32_t streamTaskSnapReaderOpen(STQ* pTq, int64_t sver, int64_t ever, SStreamTaskReader** ppReader) {
  int32_t            code = 0;
  SStreamTaskReader* pReader = NULL;

  // alloc
  pReader = (SStreamTaskReader*)taosMemoryCalloc(1, sizeof(SStreamTaskReader));
  if (pReader == NULL) {
    code = TSDB_CODE_OUT_OF_MEMORY;
    goto _err;
  }
  pReader->pTq = pTq;
  pReader->sver = sver;
  pReader->ever = ever;

  // impl
  code = tdbTbcOpen(pTq->pStreamMeta->pTaskDb, &pReader->pCur, NULL);
  if (code) {
    taosMemoryFree(pReader);
    goto _err;
  }

  code = tdbTbcMoveToFirst(pReader->pCur);
  if (code) {
    taosMemoryFree(pReader);
    goto _err;
  }

  tqInfo("vgId:%d, vnode stream-task snapshot reader opened", TD_VID(pTq->pVnode));

  *ppReader = pReader;

_err:
  tqError("vgId:%d, vnode stream-task snapshot reader open failed since %s", TD_VID(pTq->pVnode), tstrerror(code));
  *ppReader = NULL;
  return code;
  return 0;
}

int32_t streamTaskSnapReaderClose(SStreamTaskReader** ppReader) {
  int32_t code = 0;

  tdbTbcClose((*ppReader)->pCur);
  taosMemoryFree(*ppReader);
  *ppReader = NULL;

  return code;
  return 0;
}

int32_t streamTaskSnapRead(SStreamTaskReader* pReader, uint8_t** ppData) {
  int32_t     code = 0;
  const void* pKey = NULL;
  const void* pVal = NULL;
  int32_t     kLen = 0;
  int32_t     vLen = 0;
  SDecoder    decoder;
  STqHandle   handle;

  *ppData = NULL;
  for (;;) {
    if (tdbTbcGet(pReader->pCur, &pKey, &kLen, &pVal, &vLen)) {
      goto _exit;
    }

    // tDecoderInit(&decoder, (uint8_t*)pVal, vLen);
    // tDecodeSTqHandle(&decoder, &handle);
    // tDecoderClear(&decoder);

    if (handle.snapshotVer <= pReader->sver && handle.snapshotVer >= pReader->ever) {
      tdbTbcMoveToNext(pReader->pCur);
      break;
    } else {
      tdbTbcMoveToNext(pReader->pCur);
    }
  }

  *ppData = taosMemoryMalloc(sizeof(SSnapDataHdr) + vLen);
  if (*ppData == NULL) {
    code = TSDB_CODE_OUT_OF_MEMORY;
    goto _err;
  }

  SSnapDataHdr* pHdr = (SSnapDataHdr*)(*ppData);
  pHdr->type = SNAP_DATA_STREAM_TASK;
  pHdr->size = vLen;
  memcpy(pHdr->data, pVal, vLen);

  tqInfo("vgId:%d, vnode snapshot tq read data, version:%" PRId64 " subKey: %s vLen:%d", TD_VID(pReader->pTq->pVnode),
         handle.snapshotVer, handle.subKey, vLen);

_exit:
  return code;

_err:
  tqError("vgId:%d, vnode snapshot tq read data failed since %s", TD_VID(pReader->pTq->pVnode), tstrerror(code));
  return code;
  return 0;
}

// STqSnapWriter ========================================
struct SStreamTaskWriter {
  STQ*    pTq;
  int64_t sver;
  int64_t ever;
  TXN*    txn;
};

int32_t streamTaskSnapWriterOpen(STQ* pTq, int64_t sver, int64_t ever, SStreamTaskWriter** ppWriter) {
  int32_t            code = 0;
  SStreamTaskWriter* pWriter;

  // alloc
  pWriter = (SStreamTaskWriter*)taosMemoryCalloc(1, sizeof(*pWriter));
  if (pWriter == NULL) {
    code = TSDB_CODE_OUT_OF_MEMORY;
    goto _err;
  }
  pWriter->pTq = pTq;
  pWriter->sver = sver;
  pWriter->ever = ever;

  if (tdbBegin(pTq->pStreamMeta->db, &pWriter->txn, tdbDefaultMalloc, tdbDefaultFree, NULL, 0) < 0) {
    code = -1;
    taosMemoryFree(pWriter);
    goto _err;
  }

  *ppWriter = pWriter;
  return code;

_err:
  tqError("vgId:%d, stream-task snapshot writer open failed since %s", TD_VID(pTq->pVnode), tstrerror(code));
  *ppWriter = NULL;
  return code;
  return 0;
}

int32_t streamTaskSnapWriterClose(SStreamTaskWriter** ppWriter, int8_t rollback) {
  int32_t            code = 0;
  SStreamTaskWriter* pWriter = *ppWriter;
  STQ*               pTq = pWriter->pTq;

  if (rollback) {
    tdbAbort(pWriter->pTq->pStreamMeta->db, pWriter->txn);
  } else {
    code = tdbCommit(pWriter->pTq->pStreamMeta->db, pWriter->txn);
    if (code) goto _err;
    code = tdbPostCommit(pWriter->pTq->pStreamMeta->db, pWriter->txn);
    if (code) goto _err;
  }

  taosMemoryFree(pWriter);
  *ppWriter = NULL;

  // restore from metastore
  // if (tqMetaRestoreHandle(pTq) < 0) {
  //   goto _err;
  // }

  return code;

_err:
  tqError("vgId:%d, tq snapshot writer close failed since %s", TD_VID(pWriter->pTq->pVnode), tstrerror(code));
  return code;
  return 0;
}

int32_t streamTaskSnapWrite(SStreamTaskWriter* pWriter, uint8_t* pData, uint32_t nData) {
  int32_t   code = 0;
  STQ*      pTq = pWriter->pTq;
  SDecoder  decoder = {0};
  SDecoder* pDecoder = &decoder;
  STqHandle handle;

  // tDecoderInit(pDecoder, pData + sizeof(SSnapDataHdr), nData - sizeof(SSnapDataHdr));
  // code = tDecodeSTqHandle(pDecoder, &handle);
  //  if (code) goto _err;
  //  code = tqMetaSaveHandle(pTq, handle.subKey, &handle);
  //  if (code < 0) goto _err;
  //  tDecoderClear(pDecoder);
  // insert into pStreamMeta tdb table

  return code;

_err:
  tDecoderClear(pDecoder);
  tqError("vgId:%d, stream-task snapshot tq write failed since %s", TD_VID(pTq->pVnode), tstrerror(code));
  return code;
  return 0;
}
