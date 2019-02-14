MODULE_big = mongodb_fdw
OBJS = mongodb_fdw.o mongodb_option.o mongodb_query.o mongodb_connection.o $(WIN32RES)
PGFILEDESC = "mongodbdb_fdw - foreign data wrapper for MongoDB"

BSON_FLAGS=$(shell pkg-config --cflags libbson-1.0)
MONGO_FLAGS=$(shell pkg-config --cflags libmongoc-1.0)
JSON_FLAGS=$(shell pkg-config --cflags json-c)

PG_CPPFLAGS:=$(BSON_FLAGS) $(MONGO_FLAGS) $(JSON_FLAGS) $(PG_CONFIG) -Ilib
SHLIB_LINK_INTERNAL = -L. -lmongodb_client-1.0 -lmongoc-1.0

EXTENSION = mongodb_fdw
DATA = mongodb_fdw--1.0.sql

REGRESS = mongodb_fdw

ifndef LIB 
PG_CONFIG = pg_config
LIBDIR := $(shell $(PG_CONFIG) --libdir)
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
endif 

all:	libclean libcompile libinstall

libclean:
	rm -rf libmongodb_client-1.0.so lib/*.o 

libcompile:
	$(MAKE) -f lib/Makefile

libinstall:
	$(MAKE) -f lib/Makefile
	sudo /usr/bin/install -c -m 755 libmongodb_client-1.0.so $(LIBDIR)/libmongodb_client-1.0.so
