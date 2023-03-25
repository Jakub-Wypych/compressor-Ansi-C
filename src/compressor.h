/* Used for compressing/decompressing files. */

#ifndef _COMPRESSOR_H_IS_INCLUDED_
#define _COMPRESSOR_H_IS_INCLUDED_

#include "csheet.h" /* csheet_t */
#include "dictionary.h" /* dictionary_t */

void compress (dictionary_t dictionary, int bit, csheet_t csheet, FILE *in, FILE *out, unsigned char password, int VERBOSE); /* reads the 'in' file compresses it into 'out' file */

#endif
