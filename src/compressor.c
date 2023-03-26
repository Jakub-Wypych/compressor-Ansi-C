/* Used for compressing files. */

#include <stdio.h>
#include "compressor.h"
#include "dictionary.h" /* dictionary_t */
#include "counter.h" /* data_t */
#include "csheet.h" /* csheet_t */
#include "bit_funcs.h" /* bit_work_t, fbit_read, fbit_write */
#include <stdlib.h>

/* Used for compressing 'in' FILE into 'out' FILE,
 * dictionary: stores symbols with their probability,
 * bit: what was the bit reading (8, 12, 16),
 * csheet: compression sheet used for encrypting the in FILE,
 * in: uncompressed file,
 * out: compressed file,
 * password: password (duh.),
 * VERBOSE: DEBUG */
void compress(dictionary_t dictionary, int bit, csheet_t csheet, FILE *in, FILE *out, unsigned char password, int VERBOSE) {
	dictionary_t free_dict = dictionary;
	data_t found_symbol;
	csheet_t csheet_tmp = csheet;
	bit_work_t bitread = init_bitwork(in, password); /* needed for fbit_read */
	bit_work_t bitwrite = init_bitwork(out, password); /* needed for fbit_write */
	int i = 0, prob = 0;
	char buf = 0x00, transfered_bits[2];
	unsigned char ident = 0x00;
        if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Beginning to compress...\n");
	/* writing identifier for later */
	fwrite(&ident, 1, 1, out);
	/* writing dictionary */
	do {
		/* converting dictionary values into char/char* */
		transfered_bits[0] = dictionary->symbol.byte[1];
		transfered_bits[1] = dictionary->symbol.byte[0];
		buf = dictionary->probability<<4;
		fbit_write(transfered_bits, bit, bitwrite); /* writing symbol */
		fbit_write(&buf, 4, bitwrite); /* writing prob */
		if(dictionary->probability > 10) { /* writing repeating prob */
			prob = dictionary->probability;
			for(i=1;i<=prob-10;i++) {
				free_dict = dictionary;
				dictionary = dictionary->next;
				transfered_bits[0] = dictionary->symbol.byte[1];
		                transfered_bits[1] = dictionary->symbol.byte[0];
				fbit_write(transfered_bits, bit, bitwrite); /* writing symbol */
				free(free_dict);
			}
		}
		free_dict = dictionary;
		dictionary = dictionary->next;
		free(free_dict);
	} while( dictionary != NULL);
	fbit_write(transfered_bits, bit, bitwrite);  /* we've reached the end of dict, repeating the last written symbol */
	/* encrypting in FILE */
	while( fbit_read(&found_symbol, bit, bitread) != 0) {
		do {
			if(found_symbol.numeric == csheet_tmp->symbol.numeric)
				break;
		} while( (csheet_tmp = csheet_tmp->next) != NULL );
		fbit_write(csheet_tmp->code, (csheet_tmp->array_pos*8)+csheet_tmp->byte_pos, bitwrite);
		csheet_tmp = csheet;
	}
	/* adding stray bits */
	i = 8-bitwrite->buf_size; /* how many stray bits we added */
	buf = 0x00;
	if(bitwrite->buf_size!=0)
		fbit_write(&buf, 8-bitwrite->buf_size, bitwrite);
	/* setting identifier */
	ident = i<<5; /* how many stray bits were added */
	ident |= 0x01; /* compressed */
	if(bit == 16)
		ident |=0x06; /* 11 */
	else if(bit == 12)
		ident |=0x02; /* 01 */
	else /* 8 */
		ident |=0x04; /* 10 */
	if(password != 0xFF)
		ident |= 0x08;
	rewind(out);
	fwrite(&ident, 1, 1, out);
	free(bitread);
	free(bitwrite);
	if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Finished compressing!\n");
}
