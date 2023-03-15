/* Used for compressing/decompressing files. */

#ifndef _COMPRESSOR_H_IS_INCLUDED_
#define _COMPRESSOR_H_IS_INCLUDED_

#include "csheet.h" /* csheet_t */
#include "counter.h" /* data_t, heap_t */

typedef struct dictionary {
	data_t symbol;
	struct dictionary *next;
} dictionary_t;

void compress (FILE *in, csheet_t csheet, FILE *out, int VERBOSE); /* reads the 'in' file and using the csheet compresses it into 'out' file */
void decompress (FILE *in, FILE *out, int VERBOSE); /* reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */

#endif
