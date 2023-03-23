/* Used for compressing/decompressing files. */

#ifndef _COMPRESSOR_H_IS_INCLUDED_
#define _COMPRESSOR_H_IS_INCLUDED_

#include "csheet.h" /* csheet_t */
#include "counter.h" /* data_t, heap_t */

typedef struct dictionary {
	data_t symbol;
	char probability;
	struct dictionary *next;
} *dictionary_t;

dictionary_t make_dictionary(heap_t list); /* creates a dictionary from heap_t */
void compress (dictionary_t dictionary, int bit, csheet_t csheet, FILE *in, FILE *out, unsigned char password, int VERBOSE); /* reads the 'in' file compresses it into 'out' file */
void decompress (FILE *in, FILE *out, int VERBOSE); /* reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */

#endif
