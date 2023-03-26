/* Used for decompressing files. */

#ifndef _DECOMPRESSOR_H_IS_INCLUDED_
#define _DECOMPRESSOR_H_IS_INCLUDED_

void decompress (FILE *in, unsigned char password, FILE *out, int VERBOSE); /* reads the 'in' file, and decoompresses it into 'out' file */

#endif

