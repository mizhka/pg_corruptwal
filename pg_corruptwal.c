#include <errno.h>
#include <unistd.h>

#include <postgres.h>

#include <access/xlog.h>
#include <fmgr.h>
#include <funcapi.h>
#include <nodes/execnodes.h>
#include <storage/shmem.h>
#include <utils/tuplestore.h>

#include "pg_corruptwal.h"

PG_MODULE_MAGIC;

static void pg_corruptwal_hackit(struct wal_buffers_info *info, uint16 offset);
static int 	pg_corruptwal_lookup_walinfo(struct wal_buffers_info *info);
static void pg_corruptwal_return_tuple(ReturnSetInfo *rs,
									struct wal_buffers_info *info);

static int
pg_corruptwal_lookup_walinfo(struct wal_buffers_info *info)
{
	bool	found;
	Size	s;
	uint64	off;

	s = XLOGShmemSize();
	info->start = (char*)ShmemInitStruct("XLOG Ctl", s, &found);

	if (!found)
		return (-1);

	info->end = ALIGN(info->start + s);
	info->start = info->end - XLOG_BLCKSZ * XLOGbuffers;

	info->count = (info->end - info->start) / PAGESIZE;
	off = GetXLogWriteRecPtr() % (XLOG_BLCKSZ * XLOGbuffers);
	info->current = info->start + off;
	return (0);
}

static void
pg_corruptwal_hackit(struct wal_buffers_info *info, uint16 offset)
{
	char	*tmp;

#if 0
	char	 out;

	tmp = info->start;
	out = 0;

	while (tmp < info->end) {
		out ^= *tmp;
		tmp += PAGESIZE;
	}
#endif

	tmp = info->current + ((offset > 0) ? offset : 0);

//	tmp[0x100] = 0xDE;
//	tmp[0x101] = 0xAD;
//	tmp[0x102] = 0xBE;
//	tmp[0x103] = 0xEF;

	tmp[0x100] = 0xDE;
	tmp[0x101] = 0xAD;
	tmp[0x102] = 0xBE;
	tmp[0x103] = 0xEF;

	return;
}

static void
pg_corruptwal_return_tuple(ReturnSetInfo *rs, struct wal_buffers_info *info)
{
	Datum		 values[3];
	bool 		 nulls[3] = {0};

    values[0] = UInt64GetDatum(info->start);
    values[1] = UInt64GetDatum(info->current);
    values[2] = UInt64GetDatum(info->end);

    tuplestore_putvalues(rs->setResult, rs->setDesc, values, nulls);
}

PG_FUNCTION_INFO_V1(pg_corruptwal);
Datum
pg_corruptwal(PG_FUNCTION_ARGS)
{
	struct wal_buffers_info		 wal_info;
	ReturnSetInfo   			*rs;
	TupleDesc	    			 tupdesc;
	Tuplestorestate 			*tupstore;
	TypeFuncClass				 funcclass;
	MemoryContext 				 per_query_memory, old;

	/* Initialize variables */
	rs = (ReturnSetInfo *) fcinfo->resultinfo;
	per_query_memory = rs->econtext->ecxt_per_query_memory;
	old = MemoryContextSwitchTo(per_query_memory);

	tupstore = tuplestore_begin_heap(true, false, 1024);
	funcclass = get_call_result_type(fcinfo, NULL, &tupdesc);
	if (funcclass != TYPEFUNC_COMPOSITE)
		elog(ERROR, "return type must be a row type");

	rs->returnMode = SFRM_Materialize;
	rs->setResult = tupstore;
	rs->setDesc = tupdesc;

	/* Lookup addresses of pages related to XLog / WAL buffers */
	if (pg_corruptwal_lookup_walinfo(&wal_info) != 0) {
		elog(ERROR, "can't find wal location");
		goto err;
	}

	pg_corruptwal_hackit(&wal_info, (fcinfo->args[0].isnull) ? -1 :
			DatumGetUInt16(fcinfo->args[0].value));
	pg_corruptwal_return_tuple(rs, &wal_info);

err:
	tuplestore_donestoring(tupstore);
	MemoryContextSwitchTo(old);

    return (Datum) 0;
}
