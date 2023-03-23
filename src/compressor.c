/* Used for compressing/decompressing files. */

#include <stdio.h>
#include "compressor.h"
#include "counter.h" /* data_t */
#include "csheet.h" /* csheet_t */
#include "bit_funcs.h" /* var_bit_transfer, transfer_to_FILE */
#include <stdlib.h>

/* Used for compressing 'in' FILE into 'out' FILE,
 * dictionary: stores symbols with their probability,
 * bit: what was the bit reading (8, 12, 16),
 * csheet: compression sheet used for encrypting the in FILE,
 * in: uncompressed file,
 * out: compressed file,
 * VERBOSE: DEBUG */
void compress(dictionary_t dictionary, int bit, csheet_t csheet, FILE *in, FILE *out, unsigned char password, int VERBOSE) {
	dictionary_t free_dict = dictionary;
	data_t found_symbol;
	bit_work_t bitwork = init_bitwork(in, password);
	int i = 0;
	/* writing identifier for later */
	char buf = 0x00, transfered_bits[1];
	int buf_size = 0, error_count = 0;
        if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Beginning to compress...\n");
	fwrite(&buf, 1, 1, out);
	/* writing dictionary */
	do {
		transfered_bits[0] = dictionary->symbol.byte[1];
		transfered_bits[1] = dictionary->symbol.byte[0];
		buf_size = transfer_to_FILE(&buf, buf_size, transfered_bits, bit, out ); /* writing symbol */
		buf_size = var_bit_transfer(&buf, buf_size, dictionary->probability, 4, out); /* writing prob */
		if(dictionary->probability > 10) { /* writing repeating prob */
			for(i=1;i<=dictionary->probability-10;i++) {
				free_dict = dictionary;
				dictionary = dictionary->next;
				transfered_bits[0] = dictionary->symbol.byte[1];
		                transfered_bits[1] = dictionary->symbol.byte[0];
				buf_size = transfer_to_FILE(&buf, buf_size, transfered_bits, bit, out ); /* writing symbol */
				free(free_dict);
			}
		}
		if(dictionary->next == NULL) /* we've reached the end of dict, repeating the last written symbol */
			buf_size = transfer_to_FILE(&buf, buf_size, transfered_bits, bit, out );
		free_dict = dictionary;
		dictionary = dictionary->next;
		free(free_dict);
	} while( dictionary != NULL);
	/* encrypting in FILE */
	/* find symbol in in FILE, write it into found_symbol */
	/* find that symbol in csheet */
	buf_size = transfer_to_FILE(&buf, buf_size, csheet->code, (csheet->array_pos*8)+csheet->byte_pos+1, out ); /* writing code */
	/* adding stray bits */
	if(buf != 0x00)
		fwrite(&buf, 1, 1, out);
	/* setting identifier */
	/*fseek(in, 0, SEEK_SET);
	i = 8-buf_size;*/ /* how many stray bits we added */
	/*buf = i;
	buf <<= 4;*/
	/* !finish later,
	 * !replace 0x00 later */
	/*if(bit == 16)
		bit |=0x00;
	else if(bit == 12)
		bit |=0x00;
	else
		bit |=0x00;
	bit |= 0x01;*/
	free(bitwork);
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
