/*-------------------------------------------------------------------------
 *
 * mongo_wrapper.h
 * 		Foreign-data wrapper for remote MongoDB servers
 *
 * Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
 *
 * Portions Copyright (c) 2012–2014 Citus Data, Inc.
 *
 * IDENTIFICATION
 * 		mongo_wrapper.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef MONGODB_CLIENT_H
#define MONGODB_CLIENT_H

#include <mongoc.h>
#include <bson.h>
#include <bits.h>
#include <json.h>

typedef mongoc_client_t  MONGO_CONN;
typedef mongoc_cursor_t MONGO_CURSOR;
typedef bson_t BSON;

mongoc_client_t* MongoConnect(const char* host, const unsigned short port, char *databaseName, char *user, char *password,
    char *authenticationDatabase,char *replicaSet, char *readPreference,	bool ssl, char *pem_file, char *pem_pwd, char *ca_file,
    char *ca_dir, char *crl_file, bool weak_cert_validation);
void MongoDisconnect(mongoc_client_t* conn);
bool MongoInsert(mongoc_client_t* conn, char* database, char *collection, bson_t* b);
bool MongoUpdate(mongoc_client_t* conn, char* database, char *collection, bson_t* b, bson_t* op);
bool MongoDelete(mongoc_client_t* conn, char* database, char *collection, bson_t* b);
mongoc_cursor_t* MongoCursorCreate(mongoc_client_t* conn, char* database, char *collection, bson_t* q);
const bson_t* MongoCursorBson(mongoc_cursor_t* c);
bool MongoCursorNext(mongoc_cursor_t* c, bson_t* b);
void MongoCursorDestroy(mongoc_cursor_t* c);
double MongoAggregateCount(mongoc_client_t* conn, const char* database, const char* collection, const bson_t* b);

bson_t* BsonCreate(void);
void BsonDestroy(bson_t *b);

bool BsonIterInit(bson_iter_t *it, bson_t *b);
bool BsonIterSubObject(bson_iter_t *it, bson_t *b);
int32_t BsonIterInt32(bson_iter_t *it);
int64_t BsonIterInt64(bson_iter_t *it);
double BsonIterDouble(bson_iter_t *it);
bool BsonIterBool(bson_iter_t *it);
const char* BsonIterString(bson_iter_t *it);
const char* BsonIterBinData(bson_iter_t *it, uint32_t *len);
bson_oid_t * BsonIterOid(bson_iter_t *it);
time_t BsonIterDate(bson_iter_t *it);
int BsonIterType(bson_iter_t *it);
int BsonIterNext(bson_iter_t *it);
bool BsonIterSubIter(bson_iter_t *it, bson_iter_t* sub);
void BsonOidFromString(bson_oid_t *o, char* str);
void BsonOidToString(bson_oid_t *o, char* str);
const char* BsonIterCode(bson_iter_t *i);
const char* BsonIterRegex(bson_iter_t *i);
const char* BsonIterKey(bson_iter_t *i);
const char* BsonIterValue(bson_iter_t *i);

void BsonIteratorFromBuffer(bson_iter_t* i, const char * buffer);


bson_t *BsonCreate();
bool BsonAppendOid(bson_t *b, const char* key, bson_oid_t *v);
bool BsonAppendBool(bson_t *b, const char* key, bool v);
bool BsonAppendNull(bson_t *b, const char* key);
bool BsonAppendInt32(bson_t *b, const char* key, int v);
bool BsonAppendInt64(bson_t *b, const char* key, int64_t v);
bool BsonAppendDouble(bson_t *b, const char* key, double v);
bool BsonAppendUTF8(bson_t *b, const char* key, char *v);
bool BsonAppendBinary(bson_t *b, const char* key, char *v, size_t len);
bool BsonAppendDate(bson_t *b, const char* key, time_t v);
bool BsonAppendStartArray(bson_t *b, const char* key, bson_t* c);
bool BsonAppendFinishArray(bson_t *b, bson_t *c);
bool BsonAppendStartObject(bson_t* b, char *key, bson_t *r);
bool BsonAppendFinishObject(bson_t* b, bson_t* r);
bool BsonAppendBson(bson_t* b, char *key, bson_t* c);
bool BsonFinish(bson_t* b);
bool JsonToBsonAppendElement(bson_t *bb , const char *k , struct json_object *v);
json_object *JsonTokenerPrase(char * s);

char* BsonAsJson(const bson_t* bsonDocument);

int BsonToJsonStringValue(char *output, bson_iter_t *iter, bool isArray);
int DumpJsonObject(char *output, bson_iter_t *iter);
int DumpJsonArray(char *output, bson_iter_t *iter);



#endif
