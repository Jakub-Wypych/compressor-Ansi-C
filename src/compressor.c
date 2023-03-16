/* Used for compressing/decompressing files. */

#include <stdio.h>
#include "compressor.h"
#include "csheet.h" /* csheet_t */
#include <stdlib.h>

/* Reads the 'in' file and using the csheet compresses it into 'out' file */
void compress (FILE *in, csheet_t csheet, FILE *out, int VERBOSE) {
	int error_count = 0;
	if(VERBOSE)
		fprintf(stderr, "COMPRESSOR.C: Beginning to compress...\n");
	if(VERBOSE)
		fprintf(stderr, "COMPRESSOR.C: Finished compressing! Error count while compressing: %d\n", error_count);
}

/* Reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */
void decompress (FILE *in, FILE *out, int VERBOSE) {
	/*csheet_t csheet;*/
	int error_count = 0;
	if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Beginning to decompress...\n");
	if(VERBOSE)
		fprintf(stderr, "COMPRESSOR.C: Finished decompressing! Error count while decompressing: %d\n", error_count);
}
