/* Used for compressing/decompressing files. */

#include <stdio.h>
#include <compressor.h>
#include <csheet.h> /* csheet_t */
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
void check_overflow( int pos, char *binary, FILE *out) {
	if(pos>7) {
		binary_into_char(binary, out);
		pos = 0;
	}
}

/* Reads the 'in' file and using the csheet compresses it into 'out' file */
void compress (FILE *in, csheet_t csheet, FILE *out) {
	int error_count = 0;
	char data[8];
	int pos = 0; /* current position in data[8] */
	/* !write the csheet into the 'out' file,
	end it with a NULL char (00000000) */
	char c;
	while( (c = fgetc(in)) != EOF ) {
		/* searching for the char c in csheet */
		csheet_t tmp = csheet;
		while( (tmp = tmp->next) != NULL ) {
			if( c == tmp->symbol)
				break;
		}
		/* checking if we found the character */
		if(tmp != NULL) {
			/* writing in binary the length of code into 'out' file,
			e.g.: 4 = 1110; 3 = 110; 2 = 10; 1 = 0 */
			int i=0;
			while(tmp->code[i++] != NULL) {
				data[pos++] = '1';
				check_overflow(&pos, data, out);
			}
			data[pos] = '0';
			/* writing code in binary into 'out' file */
			int i=0;
			while(tmp->code[i] != NULL) {
				data[pos++] = tmp->code[i++];
				check_overflow(&pos, data, out);
			}
			while(pos!=0) { /* making sure the last byte is full */
				data[pos++]='1';
				check_overflow(&pos, data, out);
			}
		}
		else
			error_count++; /* we didn't find the char in csheet, we skip it */
	}
#ifdef VERBOSE
	fprintf(stderr, "Error count while compressing: %d\n", error_count);
#endif
}

/* Reads the 'in' file, gets the csheet from 'in' file and decoompresses it into 'out' file */
void decompress (FILE *in, FILE *out) {
	csheet_t csheet;
	/* !scan 'in' file and copy the csheet,
	a NULL character (00000000) marks the end of the csheet,
	if there are any problems with this end the process and write an error */
	int error_count = 0;
	/* !scan 'in' file until you encounter a '0',
	the length of each character found (including 0) is the length of the "code",
	look for it in csheet,
	write it's corresponding char in 'out' file and repeat */
	/* !if the string of code has a char diffrent than '0' or '1' 
	or it's not found in csheet: error_count++,
	then write the problematic string of code into 'out' file */
#ifdef VERBOSE
	fprintf(stderr, "Error count while decompressing: %d\n", error_count);
#endif
}
