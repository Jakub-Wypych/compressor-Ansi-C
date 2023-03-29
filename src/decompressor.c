/* Used for decompressing files. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */
#include "counter.h" /* data_t, heap_t */
#include "csheet.h" /* csheet_t */
#include "heap.h" /* organize_heap */
#include "bit_funcs.h" /* fbit_read, fbit_write, bit_work_t */
#include "list_iterate.h" /* read_heap, read_csheet, free_csheet_t */

/* Creates new_node and adds it to heap_t */
void add_new_node_to_heap(heap_t heap, data_t symbol, int amount) {
	heap_t new_node = malloc(sizeof(*new_node));
	new_node->symbol = symbol;
        new_node->amount = amount;
        new_node->next = NULL;
        new_node->passage_0 = NULL;
        new_node->passage_1 = NULL;
	add_to_list(heap, new_node);
}

/* Makes a copy of heap_t */
heap_t copy_heap(heap_t heap) {
	heap_t heap_copy = malloc(sizeof(*heap_copy)), heap_tmp = heap;
	heap_copy->next = NULL;
	while(heap_tmp != NULL) {
		add_new_node_to_heap(heap_copy, heap_tmp->symbol, heap_tmp->amount);
		heap_tmp = heap_tmp->next;
	}
	return heap_copy;
}

/* Frees memory and sets the exit state */
void exit_failure(bit_work_t bitwork1, bit_work_t bitwork2, FILE *file, heap_t *heap, int exit_state) {
	free(bitwork1);
	free(bitwork2);
	fclose(file);
	remove("decompressed_tmp");
	if(*heap!=NULL)
		free_heap_t(*heap);
	*heap = malloc(sizeof(*heap));
	(*heap)->amount = exit_state; /* we'll store the exit state here */
}

/* Reads the 'in' file and decoompresses it into 'out' file,
 * in: compressed file,
 * password: password,,
 * out: decompressed file,
 * VERBOSE: DEBUG. */
heap_t decompress (FILE *in, char password, FILE *out, int VERBOSE) {
        FILE *out_tmp = fopen("decompressed_tmp", "wb"); /* we first write into a tmp FILE, later on we'll copy its contents to the out FILE */
	unsigned char ident = 0x00; /* identifier */
	unsigned char stray_bits = 0; /* for reomoving added bits on the end of FILE */
	char code[MAX_CODE]; /* for decrypting the infile */
	int array_pos = 0, byte_pos = 0; /* for writing code and decrytping the infile */
	int bit = 8; /* symbol length */
	int previous_prob = 0, prob; /* probability */
	int i, stray_bytes = 0; /* how many byte we have to remove from the end of file */
	double outfile_size = 0;
	bit_work_t bitread = init_bitwork(in, password); /* needed for fbit_read */
	bit_work_t bitwrite = init_bitwork(out_tmp, 0x00); /* needed for fbit_write */
	data_t found_symbol, found_prob, previous_symbol;
	heap_t heap = malloc(sizeof(*heap)), heap_copy;
	csheet_t csheet, csheet_tmp;
	if(VERBOSE)
                fprintf(stderr, "DECOMPRESSOR.C: Beginning to decompress...\n");
	heap->next = NULL;
	/* reading identifier */
	fbit_read(&found_symbol, 8, bitread);
	ident = found_symbol.byte[1];
	if((ident&0x01) == 0) { /* checking if file is compressed */
                fprintf(stderr, "ERROR: The infile is not copmressed!\n");
		exit_failure(bitread, bitwrite, out_tmp, &heap, 209);
                return heap;
        }
	stray_bits = ident>>5;
	if( (ident&0x06) == 6 )
		bit = 16;
	else if( (ident&0x06) == 2 )
		bit = 12;
	if((ident&0x10) == 16 && password == 0x00 ) { /* is password protected and we don't have a password */
		fprintf(stderr, "ERROR: File is password protected!\n");
		exit_failure(bitread, bitwrite, out_tmp, &heap, 207);
                return heap;
	}
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
	previous_prob = prob;
	fbit_read(&found_symbol, bit, bitread); /* reading symbol (needed for while loop) */
	/* reading rest of the heap */
	while( previous_symbol.numeric != found_symbol.numeric) {
		if(fbit_read(&found_prob, 4, bitread) != 4) /* reading prob */ {
			/* we've reached the end of the file,
			 * it either means the file is damaged or we have the wrong password */
			if(password != 0x00) { /* wrong password */
				fprintf(stderr, "ERROR: The given password is incorrect!\n");
				exit_failure(bitread, bitwrite, out_tmp, &heap, 208);
			} else {
				fprintf(stderr, "ERROR: The compressed file is damaged!\n");
				exit_failure(bitread, bitwrite, out_tmp, &heap, 210);
			}
			return heap;
		}
		prob = (found_prob.byte[1]>>4);
		if(prob > 9) { /* checking for repeating prob */
			add_new_node_to_heap(heap, found_symbol, previous_prob);
			for(i=1;i<=prob-10;i++) {
				fbit_read(&found_symbol, bit, bitread); /* reading symbol */
				add_new_node_to_heap(heap, found_symbol, previous_prob);
			}
		} else {
			add_new_node_to_heap(heap, found_symbol, prob);
			previous_prob = prob;
		}
		previous_symbol = found_symbol;
		fbit_read(&found_symbol, bit, bitread); /* reading next symbol */
	}
	if(VERBOSE>1)
		read_heap(heap);
	/* creating codes for symbols */
	heap_copy = copy_heap(heap); /* copying heap for later */
	heap = organize_heap(heap, VERBOSE);
        csheet = make_cmprs_list(heap, VERBOSE);
	if(VERBOSE>1)
		read_csheet(csheet);
	/* decypting the infile */
	for(i=0;i<MAX_CODE;i++)
		code[i] = 0x00; /* filling code with 0's */
	while(fbit_read(&found_symbol, 1, bitread) == 1) { /* reading per 1 bit */
		if(found_symbol.numeric) /* bit is '1' */
			write_one_in_pos(&code[array_pos], byte_pos);
		/* looking for code in csheet */
		csheet_tmp = csheet;
		while(csheet_tmp != NULL) {
			/* checking if they have the same code and the same code length */
			if( strcmp(code, csheet_tmp->code) == 0 && array_pos == csheet_tmp->array_pos && (byte_pos+1) == csheet_tmp->byte_pos ) {
				/* we've found the symbol, writing the symbol into outfile */
				char transfered_symbol[2];
				transfered_symbol[0] = csheet_tmp->symbol.byte[1];
				transfered_symbol[1] = csheet_tmp->symbol.byte[0];
				fbit_write(transfered_symbol, bit, bitwrite);
				for(i=0;i<MAX_CODE;i++)
					code[i] = 0x00; /* filling code with 0's */
				array_pos = 0;
				byte_pos = -1;
				break;
			}
			csheet_tmp = csheet_tmp->next;
		}
		byte_pos++;
		if(byte_pos > 7) {
			byte_pos = 0;
			array_pos++;
		}
	}
	/* removing stray bits */
	byte_pos = 0;
	/* calculating how many additional bytes were added */
	for(i=0; i<=stray_bits;i++) {
		csheet_tmp = csheet;
		while(csheet_tmp != NULL) {
			if( strcmp(code, csheet_tmp->code) == 0 && csheet_tmp->array_pos == 0 && csheet_tmp->byte_pos == byte_pos++) {
				stray_bytes += bit;
				byte_pos = 0;
			}
			csheet_tmp = csheet_tmp->next;
		}
	}
	/* copying contents of out_tmp into out, without the additional bytes */
	outfile_size = ftell(out_tmp) - (stray_bytes/8);
	fclose(out_tmp);
	out_tmp = fopen("decompressed_tmp", "rb");
	while(outfile_size != 0) {
		unsigned char buf;
		fread(&buf, 1, 1, out_tmp);
		fwrite(&buf, 1, 1, out);
		outfile_size--;
	}
	fclose(out_tmp);
	free(bitread);
	free(bitwrite);
	free_csheet_t(csheet);
	remove("decompressed_tmp");
        if(VERBOSE)
                fprintf(stderr, "DECOMPRESSOR.C: Finished decompressing!\n");
	heap_copy->amount = bit; /* in first node we save the 8, 12, 16 bit reading */
	return heap_copy;
}
