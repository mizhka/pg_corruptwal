checkpoint;
begin;
insert into t values(10);
select pg_current_wal_flush_lsn (), pg_current_wal_lsn(), pg_current_wal_insert_lsn ();
select to_hex(startpos) as start, to_hex(currpos) as curr, to_hex(endpos) as end from pg_corruptwal(1);
select pg_current_wal_flush_lsn (), pg_current_wal_lsn(), pg_current_wal_insert_lsn ();
commit;
