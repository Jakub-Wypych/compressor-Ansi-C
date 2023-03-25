/* Used for decompressing files. */

#include <stdio.h>
#include <stdlib.h>

/* Reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */
void decompress (FILE *in, FILE *out, int VERBOSE) {
        if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Beginning to decompress...\n");
        if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Finished decompressing!\n");
}

