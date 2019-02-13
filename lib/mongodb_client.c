/*-------------------------------------------------------------------------
 *
 * mongodb_client.c
 * 		Foreign-data wrapper for remote MongoDB servers
 *
 * Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
 *
 * Portions Copyright (c) 2012–2014 Citus Data, Inc.
 *
 * IDENTIFICATION
 * 		mongodb_client.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <mongoc.h>
#include <json.h>

#include "mongodb_client.h"

/*
 * Connect to MongoDB server using Host/ip and Port number.
 */
mongoc_client_t*
MongoConnect(const char* host, const unsigned short port, char* databaseName, char *user, char *password,
    char *authenticationDatabase, char *replicaSet, char *readPreference, bool ssl, char *pem_file,
	char *pem_pwd, char *ca_file, char *ca_dir, char *crl_file, bool weak_cert_validation)
{
	mongoc_client_t *client = NULL;
	char* uri = NULL;

	if (user && password)
	    if (authenticationDatabase)
	        if (replicaSet)
	            if (readPreference)
		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?readPreference=%s&ssl=%s&authSource=%s&replicaSet=%s", user, password, host, port, databaseName, readPreference, ssl ? "true" : "false",authenticationDatabase,replicaSet);
		        else
		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?ssl=%s&authSource=%s&replicaSet=%s", user, password, host, port, databaseName,  ssl ? "true" : "false",authenticationDatabase,replicaSet);
		    else
   	            if (readPreference)
   		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?readPreference=%s&ssl=%s&authSource=%s", user, password, host, port, databaseName, readPreference, ssl ? "true" : "false",authenticationDatabase);
   		        else
   		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?ssl=%s&authSource=%s", user, password, host, port, databaseName,  ssl ? "true" : "false",authenticationDatabase);
		else
	        if (replicaSet)
    	        if (readPreference)
	    	        uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?readPreference=%s&ssl=%s&replicaSet=%s", user, password, host, port, databaseName, readPreference, ssl ? "true" : "false",replicaSet);
		        else
		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?ssl=%s&replicaSet=%s", user, password, host, port, databaseName,  ssl ? "true" : "false",replicaSet);
		    else
    	        if (readPreference)
	    	        uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?readPreference=%s&ssl=%s", user, password, host, port, databaseName, readPreference, ssl ? "true" : "false");
		        else
		            uri = bson_strdup_printf ("mongodb://%s:%s@%s:%hu/%s?ssl=%s", user, password, host, port, databaseName,  ssl ? "true" : "false");
	else
        if (replicaSet)
        	if (readPreference)
		        uri = bson_strdup_printf ("mongodb://%s:%hu/%s?readPreference=%s&ssl=%s&replicaSet=%s", host, port, databaseName, readPreference, ssl ? "true" : "false",replicaSet);
        	else
        		uri = bson_strdup_printf ("mongodb://%s:%hu/%s?ssl=%s&replicaSet=%s", host, port, databaseName, ssl ? "true" : "false",replicaSet);
        else
        	if (readPreference)
        	    uri = bson_strdup_printf ("mongodb://%s:%hu/%s?readPreference=%s&ssl=%s", host, port, databaseName, readPreference, ssl ? "true" : "false");
            else
            	uri = bson_strdup_printf ("mongodb://%s:%hu/%s?ssl=%s", host, port, databaseName, ssl ? "true" : "false");


	client = mongoc_client_new(uri);

	if (ssl) {
		mongoc_ssl_opt_t *ssl_opts = (mongoc_ssl_opt_t*) malloc(sizeof(mongoc_ssl_opt_t));
		ssl_opts->pem_file = pem_file;
		ssl_opts->pem_pwd = pem_pwd;
		ssl_opts->ca_file = ca_file;
		ssl_opts->ca_dir = ca_dir;
		ssl_opts->crl_file = crl_file;
		ssl_opts->weak_cert_validation = weak_cert_validation;
		mongoc_client_set_ssl_opts (client, ssl_opts);
		free(ssl_opts);
	}

	bson_free(uri);
	return client;
}

/*
 * Disconnect from MongoDB server.
 */
void
MongoDisconnect(mongoc_client_t* conn)
{
	if (conn)
		mongoc_client_destroy(conn);
}


/*
 * Insert a document 'b' into MongoDB.
 */
bool
MongoInsert(mongoc_client_t* conn, char *database, char* collection, bson_t* b)
{
	mongoc_collection_t *c = NULL;
	bson_error_t error;
	bool r = false;

	c = mongoc_client_get_collection(conn, database, collection);

	r = mongoc_collection_insert(c, MONGOC_INSERT_NONE, b, NULL, &error);
	mongoc_collection_destroy(c);
	return r;
}


/*
 * Update a document 'b' into MongoDB.
 */
bool
MongoUpdate(mongoc_client_t* conn, char* database, char *collection, bson_t* b, bson_t* op)
{
	mongoc_collection_t *c = NULL;
	bson_error_t error;
	bool r = false;

	c = mongoc_client_get_collection (conn, database, collection);

	r = mongoc_collection_update(c, MONGOC_UPDATE_NONE, b, op, NULL, &error);
	mongoc_collection_destroy(c);
	return r;
}


/*
 * Delete MongoDB's document.
 */
bool
MongoDelete(mongoc_client_t* conn, char* database, char *collection, bson_t* b)
{
	mongoc_collection_t *c = NULL;
	bson_error_t error;
	bool r = false;

	c = mongoc_client_get_collection (conn, database, collection);

	r = mongoc_collection_remove(c, MONGOC_REMOVE_SINGLE_REMOVE, b, NULL, &error);
	mongoc_collection_destroy(c);
	return r;
}

/*
 * Performs a query against the configured MongoDB server and return
 * cursor which can be destroyed by calling mongoc_cursor_current.
 */
mongoc_cursor_t*
MongoCursorCreate(mongoc_client_t* conn, char* database, char *collection, bson_t* q)
{
	mongoc_collection_t *c = NULL;
	mongoc_cursor_t *cur = NULL;
	bson_error_t error;

	c = mongoc_client_get_collection (conn, database, collection);
	cur = mongoc_collection_find(c, MONGOC_QUERY_SLAVE_OK, 0, 0, 0, q, NULL, NULL);
	mongoc_cursor_error(cur, &error);
	mongoc_collection_destroy(c);
	return cur;
}


/*
 * Destroy cursor created by calling MongoCursorCreate function.
 */
void
MongoCursorDestroy(mongoc_cursor_t* c)
{
	mongoc_cursor_destroy(c);
}


/*
 * Get the current document from cursor.
 */
const bson_t*
MongoCursorBson(mongoc_cursor_t* c)
{
	return mongoc_cursor_current(c);
}

/*
 * Get the next document from the cursor.
 */
bool
MongoCursorNext(mongoc_cursor_t* c, bson_t *b)
{
	return mongoc_cursor_next(c, (const bson_t**) &b);
}


/*
 * Allocates a new bson_t structure, and also initialize the bson
 * object. After that point objects can be appended to that bson
 * object and can be iterated. A newly allocated bson_t that should
 * be freed with bson_destroy().
 */
bson_t*
BsonCreate(void)
{
	bson_t *b = NULL;
	b = bson_new();
	bson_init(b);
	return b;
}

/*
 * Destroy Bson objected created by BsonCreate function.
 */
void
BsonDestroy(bson_t *b)
{
	bson_destroy(b);
}


/*
 * Initialize the bson Iterator.
 */
bool
BsonIterInit(bson_iter_t *it, bson_t *b)
{
	return bson_iter_init(it, b);
}


bool
BsonIterSubObject(bson_iter_t *it, bson_t *b)
{
	const uint8_t *buffer;
	uint32_t len;

	bson_iter_document(it, &len, &buffer);
	bson_init_static(b, buffer, len);
	return true;
}

int32_t
BsonIterInt32(bson_iter_t *it)
{
	return bson_iter_int32(it);
}


int64_t
BsonIterInt64(bson_iter_t *it)
{
	return bson_iter_int64(it);
}


double
BsonIterDouble(bson_iter_t *it)
{
	return bson_iter_double(it);
}


bool
BsonIterBool(bson_iter_t *it)
{
	return bson_iter_bool(it);
}


const char*
BsonIterString(bson_iter_t *it)
{
	uint32_t len = 0;
	return bson_iter_utf8(it, &len);
}

const char*
BsonIterBinData(bson_iter_t *it, uint32_t *len)
{
	const uint8_t *binary = NULL;
	bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
	bson_iter_binary (it, &subtype, len, &binary);
	return (char*)binary;
}

bson_oid_t *
BsonIterOid(bson_iter_t *it)
{
	return (bson_oid_t*)bson_iter_oid(it);
}


time_t
BsonIterDate(bson_iter_t *it)
{
	return bson_iter_date_time(it);
}


const char*
BsonIterKey(bson_iter_t *it)
{
	return bson_iter_key(it);
}

int
BsonIterType(bson_iter_t *it)
{
	return bson_iter_type(it);
}

int
BsonIterNext(bson_iter_t *it)
{
	return bson_iter_next(it);
}


bool
BsonIterSubIter(bson_iter_t *it, bson_iter_t* sub)
{
	return bson_iter_recurse(it, sub);
}


void
BsonOidFromString(bson_oid_t *o, char* str)
{
	bson_oid_init_from_string(o, str);
}


bool
BsonAppendOid(bson_t *b, const char* key, bson_oid_t *v)
{
	return bson_append_oid(b, key, strlen(key), v);
}

bool
BsonAppendBool(bson_t *b, const char* key, bool v)
{
	return bson_append_bool(b, key, -1, v);
}

bool
BsonAppendStartObject(bson_t* b, char *key, bson_t* r)
{
	return bson_append_document_begin(b, key, strlen(key), r);
}


bool
BsonAppendFinishObject(bson_t* b, bson_t* r)
{
	return bson_append_document_end(b, r);
}


bool
BsonAppendNull(bson_t *b, const char* key)
{
	return bson_append_null(b, key, strlen(key));
}


bool
BsonAppendInt32(bson_t *b, const char* key, int v)
{
	return bson_append_int32(b, key, strlen(key), v);
}


bool
BsonAppendInt64(bson_t *b, const char* key, int64_t v)
{
	return bson_append_int64(b, key, strlen(key), v);
}

bool
BsonAppendDouble(bson_t *b, const char* key, double v)
{
	return bson_append_double(b, key, strlen(key), v);
}

bool
BsonAppendUTF8(bson_t *b, const char* key, char *v)
{

	return bson_append_utf8(b, key, strlen(key), v, strlen(v));
}

bool
BsonAppendBinary(bson_t *b, const char* key, char *v, size_t len)
{
	return bson_append_binary(b, key, (int)strlen(key), BSON_SUBTYPE_BINARY, (const uint8_t *)v, len);
}

bool
BsonAppendDate(bson_t *b, const char* key, time_t v)
{
	return bson_append_date_time(b, key, strlen(key), v);
}


bool
BsonAppendBson(bson_t* b, char *key, bson_t* c)
{
	return bson_append_document(b, key, strlen(key), c);
}

bool BsonAppendStartArray(bson_t *b, const char* key, bson_t* c)
{
    return bson_append_array_begin(b, key, -1, c);
}


bool BsonAppendFinishArray(bson_t *b, bson_t* c)
{
    return bson_append_array_end(b, c);
}


bool
BsonFinish(bson_t* b)
{
	/*
	 * There is no need for bson_finish in Meta Driver.
	 * We are doing nothing, just because of compatiblity with legacy
	 * driver.
	 */
	return true;
}

bool JsonToBsonAppendElement(bson_t *bb , const char *k , struct json_object *v )
{
	bool status;

	status = true;
	if (!v)
	{
		BsonAppendNull(bb, k);
		return status;
	}

	switch (json_object_get_type(v))
	{
		case json_type_int:
		BsonAppendInt32(bb, k, json_object_get_int(v));
		break;

		case json_type_boolean:
		BsonAppendBool(bb , k, json_object_get_boolean(v));
		break;

		case json_type_double:
		BsonAppendDouble(bb, k, json_object_get_double(v));
		break;

		case json_type_string:
		BsonAppendUTF8(bb, k, (char*)json_object_get_string(v));
		break;

		case json_type_object:
		{
			bson_t t;
			struct json_object *joj = NULL;
			joj = json_object_object_get(v, "$oid");

			if (joj != NULL)
			{
				bson_oid_t bsonObjectId;
				memset(bsonObjectId.bytes, 0, sizeof(bsonObjectId.bytes));
				BsonOidFromString(&bsonObjectId, (char*)json_object_get_string(joj));
				status = BsonAppendOid(bb, k , &bsonObjectId);
				break;
			}
			joj = json_object_object_get( v, "$date" );
			if (joj != NULL)
			{
				status = BsonAppendDate(bb, k, json_object_get_int64(joj));
				break;
			}
			BsonAppendStartObject(bb , (char*)k, &t);
			json_object_object_foreach(v, kk, vv)
			{
				JsonToBsonAppendElement(&t, kk, vv);
			}
			BsonAppendFinishObject(bb, &t);
			break;
		}
		case json_type_array:
		{
			int i;
			char buf[10];
			bson_t t;
			BsonAppendStartArray(bb ,k, &t);
			for (i = 0; i < (int)json_object_array_length(v); i++)
			{
				sprintf(buf , "%d" , i);
				JsonToBsonAppendElement(&t ,buf ,json_object_array_get_idx(v, i));
			}
			BsonAppendFinishObject( bb, &t );
			break;
		}
		default:
            return false;
	}
	return status;
}

json_object*
JsonTokenerPrase(char * s)
{
	return json_tokener_parse(s);
}

/*
 * Count the number of documents.
 */
double
MongoAggregateCount(mongoc_client_t* conn, const char* database, const char* collection, const bson_t* b)
{
	bson_t            *command = NULL;
	bson_t            *reply = NULL;
	bson_t            *doc = NULL;
	double          count = 0;
	mongoc_cursor_t *cursor = NULL;
	bool            ret = false;

	command = BsonCreate();
	reply = BsonCreate();
	BsonAppendUTF8(command, "count", (char*)collection);
	if (b) /* not empty */
		BsonAppendBson(command, "query", (bson_t*)b);

	BsonFinish(command);

	cursor = mongoc_client_command(conn, database, MONGOC_QUERY_SLAVE_OK, 0, 1, 0, command, NULL, NULL);
	if (cursor)
	{
		ret = mongoc_cursor_next(cursor, (const bson_t**)&doc);
		if (ret)
		{
			bson_iter_t it;
			bson_copy_to(doc, reply);
			if (bson_iter_init_find(&it, reply, "n"))
				count = BsonIterDouble(&it);
		}
		mongoc_cursor_destroy(cursor);
	}
	BsonDestroy(reply);
	BsonDestroy(command);
	return count;
}

void
BsonIteratorFromBuffer(bson_iter_t *i, const char * buffer)
{

}

void
BsonOidToString(bson_oid_t *o, char* str)
{
	bson_oid_to_string (o, str);
}

const char*
BsonIterCode(bson_iter_t *i)
{
	return bson_iter_code (i, NULL);
}

const char*
BsonIterRegex(bson_iter_t *i)
{
	return bson_iter_regex(i, NULL);
}

const char*
BsonIterValue(bson_iter_t *i)
{
	return (char*)bson_iter_value(i);
}

int
BsonToJsonStringValue(char* output, bson_iter_t *iter, bool isArray)
{
	if (isArray)
		return DumpJsonArray(output, iter);
	else
		return DumpJsonObject(output, iter);
}

/*
 * DumpJson converts bson_t document to a JSON string.
 * isArray signifies if bsonData is contents of array or object.
 * [Some of] special bson_t datatypes are converted to JSON using
 * "Strict MongoDB Extended JSON" [1].
 *
 * [1] http://docs.mongodb.org/manual/reference/mongodb-extended-json/
 */
int
DumpJsonObject(char *output, bson_iter_t *iter)
{
	char *json;
	uint32_t len;
	const uint8_t *data = NULL;
	bson_t bson;

	bson_iter_document(iter, &len, &data);
	if (bson_init_static(&bson, data, len))
	{
		json = bson_as_json (&bson, NULL);
		if (json != NULL)
		{
            sprintf(output, "%s%s", output, json);
			bson_free(json);
		}
	}
    return strlen(output);
}

int
DumpJsonArray(char* output, bson_iter_t *iter)
{
	char *json;
	uint32_t len;
	const uint8_t *data;
	bson_t bson;

	bson_iter_array(iter, &len, &data);
	if (bson_init_static(&bson, data, len))
	{
		if((json = bson_array_as_json (&bson, NULL)))
		{
            sprintf(output, "%s%s", output, json);
			bson_free(json);
		}
	}
    return strlen(output);
}

char*
BsonAsJson(const bson_t* bsonDocument)
{
	return bson_as_json(bsonDocument, NULL);
}
