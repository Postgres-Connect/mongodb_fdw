# mongo_fdw/Makefile.meta
#
# Portions Copyright © 2004-2014, EnterpriseDB Corporation.
#
# Portions Copyright © 2012–2014 Citus Data, Inc.
#

MODULE_big = mongodb_fdw

BSON_FLAGS=$(shell pkg-config --cflags libbson-1.0)
MONGO_FLAGS=$(shell pkg-config --cflags libmongoc-1.0)
JSON_FLAGS=$(shell pkg-config --cflags json-c)

PG_CPPFLAGS:=$(BSON_FLAGS) $(MONGO_FLAGS) $(JSON_FLAGS) $(PG_CONFIG) -Ilib
SHLIB_LINK=$(shell pkg-config --libs libmongoc-1.0) -lmongodb_client-1.0 -ljson-c 

OBJS = mongodb_connection.o mongodb_option.o mongodb_fdw.o mongodb_query.o

EXTENSION = mongodb_fdw
DATA = mongodb_fdw--1.0.sql

REGRESS = mongodb_fdw
REGRESS_OPTS = --load-extension=$(EXTENSION)

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

ifndef MAJORVERSION
    MAJORVERSION := $(basename $(VERSION))
endif

ifeq (,$(findstring $(MAJORVERSION), 9.3 9.4 9.5 9.6 10.0 11.0 11beta4))
    $(error PostgreSQL version > 9.3 is required to compile this extension)
endif
