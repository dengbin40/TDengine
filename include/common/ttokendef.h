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

#ifndef _TD_COMMON_TOKEN_H_
#define _TD_COMMON_TOKEN_H_

#define TK_OR                               1
#define TK_AND                              2
#define TK_UNION                            3
#define TK_ALL                              4
#define TK_MINUS                            5
#define TK_EXCEPT                           6
#define TK_INTERSECT                        7
#define TK_NK_BITAND                        8
#define TK_NK_BITOR                         9
#define TK_NK_LSHIFT                       10
#define TK_NK_RSHIFT                       11
#define TK_NK_PLUS                         12
#define TK_NK_MINUS                        13
#define TK_NK_STAR                         14
#define TK_NK_SLASH                        15
#define TK_NK_REM                          16
#define TK_NK_CONCAT                       17
#define TK_CREATE                          18
#define TK_ACCOUNT                         19
#define TK_NK_ID                           20
#define TK_PASS                            21
#define TK_NK_STRING                       22
#define TK_ALTER                           23
#define TK_PPS                             24
#define TK_TSERIES                         25
#define TK_STORAGE                         26
#define TK_STREAMS                         27
#define TK_QTIME                           28
#define TK_DBS                             29
#define TK_USERS                           30
#define TK_CONNS                           31
#define TK_STATE                           32
#define TK_NK_COMMA                        33
#define TK_HOST                            34
#define TK_USER                            35
#define TK_ENABLE                          36
#define TK_NK_INTEGER                      37
#define TK_SYSINFO                         38
#define TK_ADD                             39
#define TK_DROP                            40
#define TK_GRANT                           41
#define TK_ON                              42
#define TK_TO                              43
#define TK_REVOKE                          44
#define TK_FROM                            45
#define TK_SUBSCRIBE                       46
#define TK_READ                            47
#define TK_WRITE                           48
#define TK_NK_DOT                          49
#define TK_WITH                            50
#define TK_ENCRYPT_KEY                     51
#define TK_DNODE                           52
#define TK_PORT                            53
#define TK_DNODES                          54
#define TK_RESTORE                         55
#define TK_NK_IPTOKEN                      56
#define TK_FORCE                           57
#define TK_UNSAFE                          58
#define TK_CLUSTER                         59
#define TK_LOCAL                           60
#define TK_QNODE                           61
#define TK_BNODE                           62
#define TK_SNODE                           63
#define TK_MNODE                           64
#define TK_VNODE                           65
#define TK_DATABASE                        66
#define TK_USE                             67
#define TK_FLUSH                           68
#define TK_TRIM                            69
#define TK_S3MIGRATE                       70
#define TK_COMPACT                         71
#define TK_IF                              72
#define TK_NOT                             73
#define TK_EXISTS                          74
#define TK_BUFFER                          75
#define TK_CACHEMODEL                      76
#define TK_CACHESIZE                       77
#define TK_COMP                            78
#define TK_DURATION                        79
#define TK_NK_VARIABLE                     80
#define TK_MAXROWS                         81
#define TK_MINROWS                         82
#define TK_KEEP                            83
#define TK_PAGES                           84
#define TK_PAGESIZE                        85
#define TK_TSDB_PAGESIZE                   86
#define TK_PRECISION                       87
#define TK_REPLICA                         88
#define TK_VGROUPS                         89
#define TK_SINGLE_STABLE                   90
#define TK_RETENTIONS                      91
#define TK_SCHEMALESS                      92
#define TK_WAL_LEVEL                       93
#define TK_WAL_FSYNC_PERIOD                94
#define TK_WAL_RETENTION_PERIOD            95
#define TK_WAL_RETENTION_SIZE              96
#define TK_WAL_ROLL_PERIOD                 97
#define TK_WAL_SEGMENT_SIZE                98
#define TK_STT_TRIGGER                     99
#define TK_TABLE_PREFIX                   100
#define TK_TABLE_SUFFIX                   101
#define TK_S3_CHUNKSIZE                   102
#define TK_S3_KEEPLOCAL                   103
#define TK_S3_COMPACT                     104
#define TK_KEEP_TIME_OFFSET               105
#define TK_ENCRYPT_ALGORITHM              106
#define TK_NK_COLON                       107
#define TK_BWLIMIT                        108
#define TK_START                          109
#define TK_TIMESTAMP                      110
#define TK_END                            111
#define TK_TABLE                          112
#define TK_NK_LP                          113
#define TK_NK_RP                          114
#define TK_STABLE                         115
#define TK_COLUMN                         116
#define TK_MODIFY                         117
#define TK_RENAME                         118
#define TK_TAG                            119
#define TK_SET                            120
#define TK_NK_EQ                          121
#define TK_USING                          122
#define TK_TAGS                           123
#define TK_PRIMARY                        124
#define TK_KEY                            125
#define TK_BOOL                           126
#define TK_TINYINT                        127
#define TK_SMALLINT                       128
#define TK_INT                            129
#define TK_INTEGER                        130
#define TK_BIGINT                         131
#define TK_FLOAT                          132
#define TK_DOUBLE                         133
#define TK_BINARY                         134
#define TK_NCHAR                          135
#define TK_UNSIGNED                       136
#define TK_JSON                           137
#define TK_VARCHAR                        138
#define TK_MEDIUMBLOB                     139
#define TK_BLOB                           140
#define TK_VARBINARY                      141
#define TK_GEOMETRY                       142
#define TK_DECIMAL                        143
#define TK_COMMENT                        144
#define TK_MAX_DELAY                      145
#define TK_WATERMARK                      146
#define TK_ROLLUP                         147
#define TK_TTL                            148
#define TK_SMA                            149
#define TK_DELETE_MARK                    150
#define TK_FIRST                          151
#define TK_LAST                           152
#define TK_SHOW                           153
#define TK_PRIVILEGES                     154
#define TK_DATABASES                      155
#define TK_TABLES                         156
#define TK_STABLES                        157
#define TK_MNODES                         158
#define TK_QNODES                         159
#define TK_ARBGROUPS                      160
#define TK_FUNCTIONS                      161
#define TK_INDEXES                        162
#define TK_ACCOUNTS                       163
#define TK_APPS                           164
#define TK_CONNECTIONS                    165
#define TK_LICENCES                       166
#define TK_GRANTS                         167
#define TK_FULL                           168
#define TK_LOGS                           169
#define TK_MACHINES                       170
#define TK_ENCRYPTIONS                    171
#define TK_QUERIES                        172
#define TK_SCORES                         173
#define TK_TOPICS                         174
#define TK_VARIABLES                      175
#define TK_BNODES                         176
#define TK_SNODES                         177
#define TK_TRANSACTIONS                   178
#define TK_DISTRIBUTED                    179
#define TK_CONSUMERS                      180
#define TK_SUBSCRIPTIONS                  181
#define TK_VNODES                         182
#define TK_ALIVE                          183
#define TK_VIEWS                          184
#define TK_VIEW                           185
#define TK_COMPACTS                       186
#define TK_NORMAL                         187
#define TK_CHILD                          188
#define TK_LIKE                           189
#define TK_TBNAME                         190
#define TK_QTAGS                          191
#define TK_AS                             192
#define TK_SYSTEM                         193
#define TK_TSMA                           194
#define TK_INTERVAL                       195
#define TK_RECURSIVE                      196
#define TK_TSMAS                          197
#define TK_FUNCTION                       198
#define TK_INDEX                          199
#define TK_COUNT                          200
#define TK_LAST_ROW                       201
#define TK_META                           202
#define TK_ONLY                           203
#define TK_TOPIC                          204
#define TK_CONSUMER                       205
#define TK_GROUP                          206
#define TK_DESC                           207
#define TK_DESCRIBE                       208
#define TK_RESET                          209
#define TK_QUERY                          210
#define TK_CACHE                          211
#define TK_EXPLAIN                        212
#define TK_ANALYZE                        213
#define TK_VERBOSE                        214
#define TK_NK_BOOL                        215
#define TK_RATIO                          216
#define TK_NK_FLOAT                       217
#define TK_OUTPUTTYPE                     218
#define TK_AGGREGATE                      219
#define TK_BUFSIZE                        220
#define TK_LANGUAGE                       221
#define TK_REPLACE                        222
#define TK_STREAM                         223
#define TK_INTO                           224
#define TK_PAUSE                          225
#define TK_RESUME                         226
#define TK_TRIGGER                        227
#define TK_AT_ONCE                        228
#define TK_WINDOW_CLOSE                   229
#define TK_IGNORE                         230
#define TK_EXPIRED                        231
#define TK_FILL_HISTORY                   232
#define TK_UPDATE                         233
#define TK_SUBTABLE                       234
#define TK_UNTREATED                      235
#define TK_KILL                           236
#define TK_CONNECTION                     237
#define TK_TRANSACTION                    238
#define TK_BALANCE                        239
#define TK_VGROUP                         240
#define TK_LEADER                         241
#define TK_MERGE                          242
#define TK_REDISTRIBUTE                   243
#define TK_SPLIT                          244
#define TK_DELETE                         245
#define TK_INSERT                         246
#define TK_NK_BIN                         247
#define TK_NK_HEX                         248
#define TK_NULL                           249
#define TK_NK_QUESTION                    250
#define TK_NK_ALIAS                       251
#define TK_NK_ARROW                       252
#define TK_ROWTS                          253
#define TK_QSTART                         254
#define TK_QEND                           255
#define TK_QDURATION                      256
#define TK_WSTART                         257
#define TK_WEND                           258
#define TK_WDURATION                      259
#define TK_IROWTS                         260
#define TK_ISFILLED                       261
#define TK_CAST                           262
#define TK_NOW                            263
#define TK_TODAY                          264
#define TK_TIMEZONE                       265
#define TK_CLIENT_VERSION                 266
#define TK_SERVER_VERSION                 267
#define TK_SERVER_STATUS                  268
#define TK_CURRENT_USER                   269
#define TK_CASE                           270
#define TK_WHEN                           271
#define TK_THEN                           272
#define TK_ELSE                           273
#define TK_BETWEEN                        274
#define TK_IS                             275
#define TK_NK_LT                          276
#define TK_NK_GT                          277
#define TK_NK_LE                          278
#define TK_NK_GE                          279
#define TK_NK_NE                          280
#define TK_MATCH                          281
#define TK_NMATCH                         282
#define TK_CONTAINS                       283
#define TK_IN                             284
#define TK_JOIN                           285
#define TK_INNER                          286
#define TK_LEFT                           287
#define TK_RIGHT                          288
#define TK_OUTER                          289
#define TK_SEMI                           290
#define TK_ANTI                           291
#define TK_ASOF                           292
#define TK_WINDOW                         293
#define TK_WINDOW_OFFSET                  294
#define TK_JLIMIT                         295
#define TK_SELECT                         296
#define TK_NK_HINT                        297
#define TK_DISTINCT                       298
#define TK_WHERE                          299
#define TK_PARTITION                      300
#define TK_BY                             301
#define TK_SESSION                        302
#define TK_STATE_WINDOW                   303
#define TK_EVENT_WINDOW                   304
#define TK_COUNT_WINDOW                   305
#define TK_SLIDING                        306
#define TK_FILL                           307
#define TK_VALUE                          308
#define TK_VALUE_F                        309
#define TK_NONE                           310
#define TK_PREV                           311
#define TK_NULL_F                         312
#define TK_LINEAR                         313
#define TK_NEXT                           314
#define TK_HAVING                         315
#define TK_RANGE                          316
#define TK_EVERY                          317
#define TK_ORDER                          318
#define TK_SLIMIT                         319
#define TK_SOFFSET                        320
#define TK_LIMIT                          321
#define TK_OFFSET                         322
#define TK_ASC                            323
#define TK_NULLS                          324
#define TK_ABORT                          325
#define TK_AFTER                          326
#define TK_ATTACH                         327
#define TK_BEFORE                         328
#define TK_BEGIN                          329
#define TK_BITAND                         330
#define TK_BITNOT                         331
#define TK_BITOR                          332
#define TK_BLOCKS                         333
#define TK_CHANGE                         334
#define TK_COMMA                          335
#define TK_CONCAT                         336
#define TK_CONFLICT                       337
#define TK_COPY                           338
#define TK_DEFERRED                       339
#define TK_DELIMITERS                     340
#define TK_DETACH                         341
#define TK_DIVIDE                         342
#define TK_DOT                            343
#define TK_EACH                           344
#define TK_FAIL                           345
#define TK_FILE                           346
#define TK_FOR                            347
#define TK_GLOB                           348
#define TK_ID                             349
#define TK_IMMEDIATE                      350
#define TK_IMPORT                         351
#define TK_INITIALLY                      352
#define TK_INSTEAD                        353
#define TK_ISNULL                         354
#define TK_MODULES                        355
#define TK_NK_BITNOT                      356
#define TK_NK_SEMI                        357
#define TK_NOTNULL                        358
#define TK_OF                             359
#define TK_PLUS                           360
#define TK_PRIVILEGE                      361
#define TK_RAISE                          362
#define TK_RESTRICT                       363
#define TK_ROW                            364
#define TK_STAR                           365
#define TK_STATEMENT                      366
#define TK_STRICT                         367
#define TK_STRING                         368
#define TK_TIMES                          369
#define TK_VALUES                         370
#define TK_VARIABLE                       371
#define TK_WAL                            372

#define TK_NK_SPACE         600
#define TK_NK_COMMENT       601
#define TK_NK_ILLEGAL       602
// #define TK_NK_HEX           603  // hex number  0x123
#define TK_NK_OCT 604  // oct number
// #define TK_NK_BIN           605  // bin format data 0b111
#define TK_BATCH_SCAN        606
#define TK_NO_BATCH_SCAN     607
#define TK_SORT_FOR_GROUP    608
#define TK_PARTITION_FIRST   609
#define TK_PARA_TABLES_SORT  610
#define TK_SMALLDATA_TS_SORT 611
#define TK_HASH_JOIN         612
#define TK_SKIP_TSMA         613

#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
