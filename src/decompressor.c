/* Used for decompressing files. */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h" /* data_t, heap_t */
#include "csheet.h" /* csheet_t */
#include "heap.h" /* organize_heap */
#include "bit_funcs.h" /* fbit_read, fbit_write, bit_work_t */
#include "list_iterate.h" /* read_heap, read_csheet, free_csheet_t */

void add_new_node_to_heap(heap_t heap, data_t symbol, int amount) {
	heap_t new_node = malloc(sizeof(*new_node));
	new_node->symbol = symbol;
        new_node->amount = amount;
        new_node->next = NULL;
        new_node->passage_0 = NULL;
        new_node->passage_1 = NULL;
	add_to_list(heap, new_node);
}

/* Reads the 'in' file and decoompresses it into 'out' file,
 * in: compressed file,
 * password: password,,
 * out: decompressed file, */
void decompress (FILE *in, unsigned char password, FILE *out, int VERBOSE) {
        unsigned char ident = 0x00; /* identifier */
	unsigned char stray_bits = 0;
	int bit = 8; /* symbol length */
	int prob;
	bit_work_t bitread = init_bitwork(in, password);
	data_t found_symbol, found_prob, previous_symbol;
	heap_t heap = malloc(sizeof(*heap));
	csheet_t csheet;
	if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Beginning to decompress...\n");
	/* reading identifier */
	fbit_read(&found_symbol, 8, bitread);
	ident = found_symbol.byte[1];
	if(ident<<7 == 0) { /* checking if file is compressed */
                fprintf(stderr, "ERROR: The infile is not copmressed!\n");
                return;
        }
	stray_bits = ident>>5;
	if( (ident&0x06) == 6 )
		bit = 16;
	else if( (ident&0x06) == 2 )
		bit = 12;
	/* !check if password protected */
	/* adding first node to heap */
	fbit_read(&found_symbol, bit, bitread); /* reading symbol */
	fbit_read(&found_prob, 4, bitread); /* reading probability */
	prob = (found_prob.byte[1]>>4);
	heap->symbol = found_symbol;
	heap->amount = prob;
	heap->next = NULL;
        heap->passage_0 = NULL;
        heap->passage_1 = NULL;
	previous_symbol = found_symbol;
	fbit_read(&found_symbol, bit, bitread); /* reading symbol (needed for while loop) */
	/* reading rest of the heap */
	while( previous_symbol.numeric != found_symbol.numeric) {
		fbit_read(&found_prob, 4, bitread); /* reading prob */
		prob = (found_prob.byte[1]>>4);
		if(prob > 9) { /* checking for repeating prob */
			int previous_prob = heap->amount;
			int i;
			add_new_node_to_heap(heap, found_symbol, previous_prob);
			for(i=1;i<=prob-10;i++) {
				fbit_read(&found_symbol, bit, bitread); /* reading symbol */
				add_new_node_to_heap(heap, found_symbol, previous_prob);
			}
		} else
			add_new_node_to_heap(heap, found_symbol, prob);
		previous_symbol = found_symbol;
		fbit_read(&found_symbol, bit, bitread); /* reading next symbol */
	}
	if(VERBOSE>1)
		read_heap(heap);
	/* creating codes for symbols */
	heap = organize_heap(heap, VERBOSE);
        csheet = make_cmprs_list(heap, VERBOSE);
	if(VERBOSE>1)
		read_csheet(csheet);
	/* decypting the infile */
	free(bitread);
	free_csheet_t(csheet);
        if(VERBOSE)
                fprintf(stderr, "COMPRESSOR.C: Finished decompressing!\n");
}

