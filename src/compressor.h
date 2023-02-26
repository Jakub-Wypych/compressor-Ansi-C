/* Used for compressing/decompressing files. */

#ifndef _COMPRESSOR_H_IS_INCLUDED_
#define _COMPRESSOR_H_IS_INCLUDED_

#include "csheet.h" /* csheet_t */

void binary_into_char (char *binary, FILE *out); /* converts binary code into a character and writes it in 'out' file */
void char_into_binary(char c, char *binary); /* converts character into binary code */
void compress (FILE *in, csheet_t csheet, FILE *out); /* reads the 'in' file and using the csheet compresses it into 'out' file */
void decompress (FILE *in, FILE *out); /* reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */

#endif
