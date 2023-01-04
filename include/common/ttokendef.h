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

#define TK_OR                   1
#define TK_AND                  2
#define TK_UNION                3
#define TK_ALL                  4
#define TK_MINUS                5
#define TK_EXCEPT               6
#define TK_INTERSECT            7
#define TK_NK_BITAND            8
#define TK_NK_BITOR             9
#define TK_NK_LSHIFT            10
#define TK_NK_RSHIFT            11
#define TK_NK_PLUS              12
#define TK_NK_MINUS             13
#define TK_NK_STAR              14
#define TK_NK_SLASH             15
#define TK_NK_REM               16
#define TK_NK_CONCAT            17
#define TK_CREATE               18
#define TK_ACCOUNT              19
#define TK_NK_ID                20
#define TK_PASS                 21
#define TK_NK_STRING            22
#define TK_ALTER                23
#define TK_PPS                  24
#define TK_TSERIES              25
#define TK_STORAGE              26
#define TK_STREAMS              27
#define TK_QTIME                28
#define TK_DBS                  29
#define TK_USERS                30
#define TK_CONNS                31
#define TK_STATE                32
#define TK_USER                 33
#define TK_ENABLE               34
#define TK_NK_INTEGER           35
#define TK_SYSINFO              36
#define TK_DROP                 37
#define TK_GRANT                38
#define TK_ON                   39
#define TK_TO                   40
#define TK_REVOKE               41
#define TK_FROM                 42
#define TK_SUBSCRIBE            43
#define TK_NK_COMMA             44
#define TK_READ                 45
#define TK_WRITE                46
#define TK_NK_DOT               47
#define TK_DNODE                48
#define TK_PORT                 49
#define TK_DNODES               50
#define TK_NK_IPTOKEN           51
#define TK_FORCE                52
#define TK_LOCAL                53
#define TK_QNODE                54
#define TK_BNODE                55
#define TK_SNODE                56
#define TK_MNODE                57
#define TK_DATABASE             58
#define TK_USE                  59
#define TK_FLUSH                60
#define TK_TRIM                 61
#define TK_COMPACT              62
#define TK_IF                   63
#define TK_NOT                  64
#define TK_EXISTS               65
#define TK_BUFFER               66
#define TK_CACHEMODEL           67
#define TK_CACHESIZE            68
#define TK_COMP                 69
#define TK_DURATION             70
#define TK_NK_VARIABLE          71
#define TK_MAXROWS              72
#define TK_MINROWS              73
#define TK_KEEP                 74
#define TK_PAGES                75
#define TK_PAGESIZE             76
#define TK_TSDB_PAGESIZE        77
#define TK_PRECISION            78
#define TK_REPLICA              79
#define TK_VGROUPS              80
#define TK_SINGLE_STABLE        81
#define TK_RETENTIONS           82
#define TK_SCHEMALESS           83
#define TK_WAL_LEVEL            84
#define TK_WAL_FSYNC_PERIOD     85
#define TK_WAL_RETENTION_PERIOD 86
#define TK_WAL_RETENTION_SIZE   87
#define TK_WAL_ROLL_PERIOD      88
#define TK_WAL_SEGMENT_SIZE     89
#define TK_STT_TRIGGER          90
#define TK_TABLE_PREFIX         91
#define TK_TABLE_SUFFIX         92
#define TK_NK_COLON             93
#define TK_MAX_SPEED            94
#define TK_TABLE                95
#define TK_NK_LP                96
#define TK_NK_RP                97
#define TK_STABLE               98
#define TK_ADD                  99
#define TK_COLUMN               100
#define TK_MODIFY               101
#define TK_RENAME               102
#define TK_TAG                  103
#define TK_SET                  104
#define TK_NK_EQ                105
#define TK_USING                106
#define TK_TAGS                 107
#define TK_COMMENT              108
#define TK_BOOL                 109
#define TK_TINYINT              110
#define TK_SMALLINT             111
#define TK_INT                  112
#define TK_INTEGER              113
#define TK_BIGINT               114
#define TK_FLOAT                115
#define TK_DOUBLE               116
#define TK_BINARY               117
#define TK_TIMESTAMP            118
#define TK_NCHAR                119
#define TK_UNSIGNED             120
#define TK_JSON                 121
#define TK_VARCHAR              122
#define TK_MEDIUMBLOB           123
#define TK_BLOB                 124
#define TK_VARBINARY            125
#define TK_DECIMAL              126
#define TK_MAX_DELAY            127
#define TK_WATERMARK            128
#define TK_ROLLUP               129
#define TK_TTL                  130
#define TK_SMA                  131
#define TK_DELETE_MARK          132
#define TK_FIRST                133
#define TK_LAST                 134
#define TK_SHOW                 135
#define TK_PRIVILEGES           136
#define TK_DATABASES            137
#define TK_TABLES               138
#define TK_STABLES              139
#define TK_MNODES               140
#define TK_QNODES               141
#define TK_FUNCTIONS            142
#define TK_INDEXES              143
#define TK_ACCOUNTS             144
#define TK_APPS                 145
#define TK_CONNECTIONS          146
#define TK_LICENCES             147
#define TK_GRANTS               148
#define TK_QUERIES              149
#define TK_SCORES               150
#define TK_TOPICS               151
#define TK_VARIABLES            152
#define TK_CLUSTER              153
#define TK_BNODES               154
#define TK_SNODES               155
#define TK_TRANSACTIONS         156
#define TK_DISTRIBUTED          157
#define TK_CONSUMERS            158
#define TK_SUBSCRIPTIONS        159
#define TK_VNODES               160
#define TK_ALIVE                161
#define TK_LIKE                 162
#define TK_TBNAME               163
#define TK_QTAGS                164
#define TK_AS                   165
#define TK_INDEX                166
#define TK_FUNCTION             167
#define TK_INTERVAL             168
#define TK_COUNT                169
#define TK_LAST_ROW             170
#define TK_TOPIC                171
#define TK_WITH                 172
#define TK_META                 173
#define TK_CONSUMER             174
#define TK_GROUP                175
#define TK_DESC                 176
#define TK_DESCRIBE             177
#define TK_RESET                178
#define TK_QUERY                179
#define TK_CACHE                180
#define TK_EXPLAIN              181
#define TK_ANALYZE              182
#define TK_VERBOSE              183
#define TK_NK_BOOL              184
#define TK_RATIO                185
#define TK_NK_FLOAT             186
#define TK_OUTPUTTYPE           187
#define TK_AGGREGATE            188
#define TK_BUFSIZE              189
#define TK_STREAM               190
#define TK_INTO                 191
#define TK_TRIGGER              192
#define TK_AT_ONCE              193
#define TK_WINDOW_CLOSE         194
#define TK_IGNORE               195
#define TK_EXPIRED              196
#define TK_FILL_HISTORY         197
#define TK_SUBTABLE             198
#define TK_KILL                 199
#define TK_CONNECTION           200
#define TK_TRANSACTION          201
#define TK_BALANCE              202
#define TK_VGROUP               203
#define TK_MERGE                204
#define TK_REDISTRIBUTE         205
#define TK_SPLIT                206
#define TK_DELETE               207
#define TK_INSERT               208
#define TK_NULL                 209
#define TK_NK_QUESTION          210
#define TK_NK_ARROW             211
#define TK_ROWTS                212
#define TK_QSTART               213
#define TK_QEND                 214
#define TK_QDURATION            215
#define TK_WSTART               216
#define TK_WEND                 217
#define TK_WDURATION            218
#define TK_IROWTS               219
#define TK_ISFILLED             220
#define TK_CAST                 221
#define TK_NOW                  222
#define TK_TODAY                223
#define TK_TIMEZONE             224
#define TK_CLIENT_VERSION       225
#define TK_SERVER_VERSION       226
#define TK_SERVER_STATUS        227
#define TK_CURRENT_USER         228
#define TK_CASE                 229
#define TK_END                  230
#define TK_WHEN                 231
#define TK_THEN                 232
#define TK_ELSE                 233
#define TK_BETWEEN              234
#define TK_IS                   235
#define TK_NK_LT                236
#define TK_NK_GT                237
#define TK_NK_LE                238
#define TK_NK_GE                239
#define TK_NK_NE                240
#define TK_MATCH                241
#define TK_NMATCH               242
#define TK_CONTAINS             243
#define TK_IN                   244
#define TK_JOIN                 245
#define TK_INNER                246
#define TK_SELECT               247
#define TK_DISTINCT             248
#define TK_WHERE                249
#define TK_PARTITION            250
#define TK_BY                   251
#define TK_SESSION              252
#define TK_STATE_WINDOW         253
#define TK_EVENT_WINDOW         254
#define TK_START                255
#define TK_SLIDING              256
#define TK_FILL                 257
#define TK_VALUE                258
#define TK_NONE                 259
#define TK_PREV                 260
#define TK_LINEAR               261
#define TK_NEXT                 262
#define TK_HAVING               263
#define TK_RANGE                264
#define TK_EVERY                265
#define TK_ORDER                266
#define TK_SLIMIT               267
#define TK_SOFFSET              268
#define TK_LIMIT                269
#define TK_OFFSET               270
#define TK_ASC                  271
#define TK_NULLS                272
#define TK_ABORT                273
#define TK_AFTER                274
#define TK_ATTACH               275
#define TK_BEFORE               276
#define TK_BEGIN                277
#define TK_BITAND               278
#define TK_BITNOT               279
#define TK_BITOR                280
#define TK_BLOCKS               281
#define TK_CHANGE               282
#define TK_COMMA                283
#define TK_CONCAT               284
#define TK_CONFLICT             285
#define TK_COPY                 286
#define TK_DEFERRED             287
#define TK_DELIMITERS           288
#define TK_DETACH               289
#define TK_DIVIDE               290
#define TK_DOT                  291
#define TK_EACH                 292
#define TK_FAIL                 293
#define TK_FILE                 294
#define TK_FOR                  295
#define TK_GLOB                 296
#define TK_ID                   297
#define TK_IMMEDIATE            298
#define TK_IMPORT               299
#define TK_INITIALLY            300
#define TK_INSTEAD              301
#define TK_ISNULL               302
#define TK_KEY                  303
#define TK_MODULES              304
#define TK_NK_BITNOT            305
#define TK_NK_SEMI              306
#define TK_NOTNULL              307
#define TK_OF                   308
#define TK_PLUS                 309
#define TK_PRIVILEGE            310
#define TK_RAISE                311
#define TK_REPLACE              312
#define TK_RESTRICT             313
#define TK_ROW                  314
#define TK_SEMI                 315
#define TK_STAR                 316
#define TK_STATEMENT            317
#define TK_STRICT               318
#define TK_STRING               319
#define TK_TIMES                320
#define TK_UPDATE               321
#define TK_VALUES               322
#define TK_VARIABLE             323
#define TK_VIEW                 324
#define TK_WAL                  325

#define TK_NK_SPACE   600
#define TK_NK_COMMENT 601
#define TK_NK_ILLEGAL 602
#define TK_NK_HEX     603  // hex number  0x123
#define TK_NK_OCT     604  // oct number
#define TK_NK_BIN     605  // bin format data 0b111

#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
