/* Used for compressing/decompressing files. */

#ifndef _COMPRESSOR_H_IS_INCLUDED_
#define _COMPRESSOR_H_IS_INCLUDED_

void compress (FILE *in, csheet_t csheet, FILE *out); /* reads the 'in' file and using the csheet compresses it into 'out' file */
void decompress (FILE *in, FILE *out); /* reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */

#endif
