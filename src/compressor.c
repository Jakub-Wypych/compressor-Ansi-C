/* Used for compressing/decompressing files. */

#include <stdio.h>
#include "compressor.h"
#include "csheet.h" /* csheet_t */
#include <stdlib.h> /* fprintf, strtol */

/* Converts binary code into a character and writes it in 'out' file */
void binary_into_char (char *binary, FILE *out) {
	char c = strtol(binary, 0, 2);
	fprintf(out, "%c", c);
}

/* Converts character into binary code */
void char_into_binary(char c, char *binary) { /* *binary has to be binary[8] */
        int i, j;
        j = 0;
        for(i = 7; i >= 0; i-- )
                binary[j++] = ( c & ( 1 << i )) ? '1' : '0';
}

/* Checks if binary is full and if so, write it as a char into 'out' file */
int check_overflow( int pos, char *binary, FILE *out) {
	if(pos>7) {
		binary_into_char(binary, out);
		return 1;
	}
	return 0;
}

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
	/* !scan 'in' file and copy the csheet,
	a NULL character (00000000) marks the end of the csheet,
	if there are any problems with this end the process and write an error */
	/* !scan 'in' file until you encounter a '0',
	the length of each character found (including 0) is the length of the "code",
	look for it in csheet,
	write it's corresponding char in 'out' file and repeat */
	/* !if the string of code has a char diffrent than '0' or '1' 
	or it's not found in csheet: error_count++,
	then write the problematic string of code into 'out' file */
	if(VERBOSE)
		fprintf(stderr, "COMPRESSOR.C: Finished decompressing! Error count while decompressing: %d\n", error_count);
}
