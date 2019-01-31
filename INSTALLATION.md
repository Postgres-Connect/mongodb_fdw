Installation
------------
Number of manual steps needs to be performed to compile and install different type of MongoDB drivers and supported libraries. If you want to avoid the manual steps, there is a shell script available which will download and install the appropriate drivers and libraries for you.

Here is how it works :

* autogen.sh
Downloading mongoc driver ...
  -  rm -rf lib/mongo-c-driver  OK
  -  wget https://github.com/mongodb/mongo-c-driver/releases/download/1.9.5/mongo-c-driver-1.9.5.tar.gz -P lib/  OK
  -  tar -zxvf lib/mongo-c-driver-1.9.5.tar.gz -C lib/  OK
  -  mv lib/mongo-c-driver-1.9.5 lib/mongo-c-driver  OK
  -  rm -rf lib/mongo-c-driver-1.9.5.tar.gz  OK
Downloading json library ...
  -  rm -rf lib/json-c  OK
  -  wget https://github.com/json-c/json-c/archive/json-c-0.13.1-20180305.tar.gz -P lib/  OK
  -  tar -zxvf lib/json-c-0.13.1-20180305.tar.gz -C lib/  OK
  -  mv lib/json-c-json-c-0.13.1-20180305 lib/json-c  OK
  -  rm -rf lib/json-c-0.13.1-20180305.tar.gz  OK
Installing mongoc driver ...
  -  ./configure --with-libbson=auto --enable-ssl  OK
  -  make install  OK
Installing json library ...
  -  ./autogen.sh  OK
  -  ./configure CFLAGS='-fPIC'  OK
  -  make install  OK
Compiling/Installing mongodb_fdw ...
  -  make install  OK
Finishid

