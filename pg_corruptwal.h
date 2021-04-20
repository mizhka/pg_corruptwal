/*
 * pg_numamove.h
 *
 *  Created on: Feb 11, 2021
 *      Author: mizhka
 */

#ifndef PG_NUMAMOVE_H_
#define PG_NUMAMOVE_H_

#include <sys/types.h>

#define	ALIGN(ptr)				((char *) ((uintptr_t) ptr & ~(uintptr_t) (PAGESIZE - 1)))
#define BIT_CHECK(mask, bit)	(((mask) & ((int)1 << (bit))) != 0)
#define BIT_CLEAR(mask, bit)	(mask &= ~((int)1 << (bit)))
#define BIT_SET(mask, bit)		(mask |= ((int)1 << (bit)))
#define	IS_UNALIGNED(ptr)		((uintptr_t) ptr & ~(uintptr_t) (PAGESIZE - 1))
#define	PAGESIZE				0x1000

struct wal_buffers_info {
	char	*start;
	char 	*current;
	char	*end;
	int		 count;
};

#endif /* PG_NUMAMOVE_H_ */
