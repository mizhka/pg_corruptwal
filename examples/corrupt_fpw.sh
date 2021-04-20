#!/bin/sh

cd /var/db/postgres/
service postgresql onestop
rm -rf /var/db/postgres/data13
service postgresql oneinitdb
service postgresql onestart
psql -U postgres -f /root/pg_corruptwal/examples/create.sql
psql -U postgres -f /root/pg_corruptwal/examples/corrupt_fpw.sql
cd /var/db/postgres/data13/pg_wal
pg_waldump  000000010000000000000001 2>&1 | tail 

