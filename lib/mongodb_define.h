#ifndef MONGODB_DEFINE_H
#define MONGODB_DEFINE_H

#include <mongoc.h>
	#define BSON bson_t
	#define BSON_TYPE bson_type_t
	#define BSON_ITERATOR bson_iter_t
	#define MONGO_CONN mongoc_client_t
	#define MONGO_CURSOR mongoc_cursor_t
	#define BSON_TYPE_DOCUMENT BSON_TYPE_DOCUMENT
	#define BSON_TYPE_NULL BSON_TYPE_NULL
	#define BSON_TYPE_ARRAY BSON_TYPE_ARRAY
	#define BSON_TYPE_INT32 BSON_TYPE_INT32
	#define BSON_TYPE_INT64 BSON_TYPE_INT64
	#define BSON_TYPE_DOUBLE BSON_TYPE_DOUBLE
	#define BSON_TYPE_BINDATA BSON_TYPE_BINARY
	#define BSON_TYPE_BOOL BSON_TYPE_BOOL
	#define BSON_TYPE_UTF8 BSON_TYPE_UTF8
	#define BSON_TYPE_OID BSON_TYPE_OID
	#define BSON_TYPE_DATE_TIME BSON_TYPE_DATE_TIME
	#define BSON_TYPE_SYMBOL BSON_TYPE_SYMBOL
	#define BSON_TYPE_UNDEFINED BSON_TYPE_UNDEFINED
	#define BSON_TYPE_REGEX BSON_TYPE_REGEX
	#define BSON_TYPE_CODE BSON_TYPE_CODE
	#define BSON_TYPE_CODEWSCOPE BSON_TYPE_CODEWSCOPE
	#define BSON_TYPE_TIMESTAMP BSON_TYPE_TIMESTAMP

	#define PREF_READ_PRIMARY_NAME "readPrimary"
	#define PREF_READ_SECONDARY_NAME "readSecondary"
	#define PREF_READ_PRIMARY_PREFERRED_NAME "readPrimaryPreferred"
	#define PREF_READ_SECONDARY_PREFERRED_NAME "readSecondaryPreferred"
	#define PREF_READ_NEAREST_NAME "readNearest"

	#define BSON_ITER_BOOL bson_iter_bool
	#define BSON_ITER_DOUBLE bson_iter_double
	#define BSON_ITER_INT32 bson_iter_int32
	#define BSON_ITER_INT64 bson_iter_int64
	#define BSON_ITER_OID bson_iter_oid
	#define BSON_ITER_UTF8 bson_iter_utf8
	#define BSON_ITER_REGEX bson_iter_regex
	#define BSON_ITER_DATE_TIME bson_iter_date_time
	#define BSON_ITER_CODE bson_iter_code
	#define BSON_ITER_VALUE bson_iter_value
	#define BSON_ITER_KEY bson_iter_key
	#define BSON_ITER_NEXT bson_iter_next
	#define BSON_ITER_TYPE bson_iter_type
	#define BSON_ITER_BINARY bson_iter_binary
#endif
