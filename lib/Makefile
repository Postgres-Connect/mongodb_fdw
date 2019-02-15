.PHONY : clean

CC=gcc
#CPPFLAGS:= -O0 -g3 -Wno-unused-variable -fPIC -Wall -I. -Ilib -Imongo-c-driver/src/mongoc/ -Ijson-c/ -Imongo-c-driver/src/libbson/src/bson $(pkg-config --cflags libbson-1.0)

BSON_FLAGS=$(shell pkg-config --cflags libbson-1.0)
MONGO_FLAGS=$(shell pkg-config --cflags libmongoc-1.0)
JSON_FLAGS=$(shell pkg-config --cflags json-c)
PG_CONFIG=-I$(shell pg_config --includedir)

CPPFLAGS:= -Wno-unused-variable -fPIC -Wall $(BSON_FLAGS) $(MONGO_FLAGS) $(JSON_FLAGS) $(PG_CONFIG)
 
LDFLAGS:= $(LDFLAGS) -fPIC -shared -ldl $(shell pkg-config --libs libmongoc-1.0) -ljson-c

PG_CONFIG=pg_config
INSTALL_DIR=$(shell $(PG_CONFIG) --libdir)

SOURCES = lib/mongodb_client.c
HEADERS = 
OBJECTS = $(SOURCES:.c=.o)

TARGET=libmongodb_client-1.0.so

all: $(TARGET) 

clean:
	rm -rf $(OBJECTS) $(TARGET) 

install:
	cp -rf $(TARGET) $(INSTALL_DIR)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

