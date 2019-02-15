#! /bin/bash

#-------------------------------------------------------------------------
#
# autogen.sh
#             Foreign-data wrapper for remote MongoDB servers
#
# Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
#
# Portions Copyright (c) 2004-2014, EnterpriseDB Corporation.
#
# IDENTIFICATION
#             autogen.sh
#
#-------------------------------------------------------------------------


MONGOC_VERSION=1.9.5
JSONC_VERSION=0.13.1-20180305


function run_cmd()
{
    $1&>logfile
    if [ $? ]
    then
        echo  "  - " $1 " OK"
    else
        echo  "  - " $1 " FAILED"
        echo "Failed, look at the logfile"
        exit 1
    fi 
}

###
# Pull the latest version of Monggo C Driver's master branch
#
function checkout_mongo_driver
{
    run_cmd "rm -rf lib/mongo-c-driver" 
	run_cmd "wget https://github.com/mongodb/mongo-c-driver/releases/download/$MONGOC_VERSION/mongo-c-driver-$MONGOC_VERSION.tar.gz -P lib/"
	run_cmd "tar -zxvf lib/mongo-c-driver-$MONGOC_VERSION.tar.gz -C lib/"
	run_cmd "mv lib/mongo-c-driver-$MONGOC_VERSION lib/mongo-c-driver"
	run_cmd "rm -rf lib/mongo-c-driver-$MONGOC_VERSION.tar.gz"
}

##
# Pull the json-c library
#
function checkout_json_lib
{
	run_cmd "rm -rf lib/json-c"
	run_cmd "wget https://github.com/json-c/json-c/archive/json-c-$JSONC_VERSION.tar.gz -P lib/"
	run_cmd "tar -zxvf lib/json-c-$JSONC_VERSION.tar.gz -C lib/"
	run_cmd "mv lib/json-c-json-c-$JSONC_VERSION lib/json-c"
	run_cmd "rm -rf lib/json-c-$JSONC_VERSION.tar.gz"
}


##
# Compile and instal json-c library
#
function install_json_lib
{
    cd lib/json-c
	run_cmd "./autogen.sh"
	run_cmd "./configure"
	run_cmd "make"
	run_cmd "sudo -s make install"
    cd ../../
}

###
# Configure and install the Mongo C Driver and libbson
#
function install_mongoc_driver
{
    cd lib/mongo-c-driver/
	run_cmd "./configure --with-libbson=auto --enable-ssl --prefix=/usr/local/"
	run_cmd "make"
	run_cmd "sudo -s make install"
    cd ../../
}

function install_mongodb_fdw 
{
    make
    make install
}

echo "Downloading mongoc driver ..."
checkout_mongo_driver
echo "Downloading json library ..."
checkout_json_lib
echo "Installing mongoc driver ..."
install_mongoc_driver
echo "Installing json library ..."
install_json_lib
echo "Compiling/Installing mongodb_fdw ..."
install_mongodb_fdw 
export PKG_CONFIG_PATH=lib/mongo-c-driver/src/:lib/mongo-c-driver/src/libbson/src
echo "Finishid"
