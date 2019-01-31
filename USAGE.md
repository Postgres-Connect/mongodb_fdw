Usage
-----
The following parameters can be set on a MongoDB foreign server object:

  * **`address`**: the address or hostname of the MongoDB server Defaults to `127.0.0.1`
  * **`port`**: the port number of the MongoDB server. Defaults to `27017`
  * **`authentication_database`**: database against which user will be authenticated against. Only valid with password based authentication. Defaults to per same database as the MongoDB collection database.
  * **`replica_set`**: replica set the server is member of. If set, driver will auto-connect to correct primary in the replica set when writing.  
  * **`read_preference`**: primary [default], secondary, primaryPreferred, secondaryPreferred, or nearest (meta driver only).  Defaults to `primary`
  * **`ssl`**: false [default], true to enable ssl (meta driver only). See http://mongoc.org/libmongoc/current/mongoc_ssl_opt_t.html to understand the options.
  * **`pem_file`**: SSL option;
  * **`pem_pwd`**: SSL option;
  * **`ca_file`**: SSL option;
  * **`ca_dir`**: SSL option;
  * **`crl_file`**: SSL option;
  * **`weak_cert_validation`**: SSL option;

The following parameters can be set on a MongoDB foreign table object:

  * **`database`**: the name of the MongoDB database to query. Defaults to `test`
  * **`collection`**: the name of the MongoDB collection to query. Defaults to the foreign table name used in the relevant `CREATE` command

As an example, the following commands demonstrate loading the `mongo_fdw`
wrapper, creating a server, and then creating a foreign table associated with
a MongoDB collection. The commands also show specifying option values in the
`OPTIONS` clause. If an option value isn't provided, the wrapper uses the
default value mentioned above.

`mongo_fdw` can collect data distribution statistics will incorporate them when estimating costs for the query execution plan. To see selected execution plans for a query, just run `EXPLAIN`.

Examples with MongoDB's equivalent statments.

```sql

-- load extension first time after install
CREATE EXTENSION mongo_fdw;

-- create server object
CREATE SERVER mongo_server
         FOREIGN DATA WRAPPER mongo_fdw
         OPTIONS (address '127.0.0.1', port '27017');

-- create user mapping
CREATE USER MAPPING FOR postgres
		 SERVER mongo_server
		 OPTIONS (username 'mongo_user', password 'mongo_pass');

-- create foreign table
CREATE FOREIGN TABLE warehouse(
		 _id NAME,
         warehouse_id int,
         warehouse_name text,
         warehouse_created timestamptz)
SERVER mongo_server
         OPTIONS (database 'db', collection 'warehouse');

-- Note: first column of the table must be "_id" of type "NAME".

-- select from table
SELECT * FROM warehouse WHERE warehouse_id = 1;

           _id          | warehouse_id | warehouse_name |     warehouse_created
------------------------+----------------+---------------------------
53720b1904864dc1f5a571a0|            1 | UPS            | 12-DEC-14 12:12:10 +05:00


db.warehouse.find({"warehouse_id" : 1}).pretty()
{
	"_id" : ObjectId("53720b1904864dc1f5a571a0"),
	"warehouse_id" : 1,
	"warehouse_name" : "UPS",
	"warehouse_created" : ISODate("2014-12-12T07:12:10Z")
}


-- insert row in table
INSERT INTO warehouse values (0, 1, 'UPS', '2014-12-12T07:12:10Z');

db.warehouse.insert
(
    {
        "warehouse_id" : NumberInt(1),
        "warehouse_name" : "UPS",
        "warehouse_created" : ISODate("2014-12-12T07:12:10Z")
    }
);

-- delete row from table
DELETE FROM warehouse where warehouse_id = 3;

>    db.warehouse.remove({"warehouse_id" : 2})


-- update a row of table
UPDATE warehouse set warehouse_name = 'UPS_NEW' where warehouse_id = 1;

db.warehouse.update
(
   {
        "warehouse_id" : 1
   },
   {
        "warehouse_id" : 1,
        "warehouse_name" : "UPS_NEW"
   }
)

-- explain a table
EXPLAIN SELECT * FROM warehouse WHERE warehouse_id = 1;
                           QUERY PLAN
 -----------------------------------------------------------------
 Foreign Scan on warehouse  (cost=0.00..0.00 rows=1000 width=44)
   Filter: (warehouse_id = 1)
   Foreign Namespace: db.warehouse
 Planning time: 0.671 ms
(4 rows)

-- collect data distribution statistics`
ANALYZE warehouse;

select mongo_fdw_version();
 mongo_fdw_version
-------------------
             50100
(1 row)

