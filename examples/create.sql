create extension pg_corruptwal;
create table t(id int);
insert into t select x from generate_series(1,10000) x;

