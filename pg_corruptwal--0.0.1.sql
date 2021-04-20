-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_corruptwal" to load this file. \quit
CREATE FUNCTION pg_corruptwal(offs IN integer, startpos OUT bigint, currpos OUT bigint, endpos OUT bigint) 
RETURNS SETOF RECORD
AS '$libdir/pg_corruptwal'
LANGUAGE C VOLATILE;
