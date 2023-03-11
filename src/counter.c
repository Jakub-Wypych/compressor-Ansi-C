/* Used for counting how many times a symbol occurs in a file.
 * Returns a list of symbols and how many times they occured */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"
#include "utility.h"

void increase_amount_in_list(heap_t list, data_t c) {
        heap_t new_node, tmp = list;
        do {
                if(c.numeric == tmp->symbol.numeric) {
                        tmp->amount++;
                        return;
                }
        } while( (tmp = tmp->next) != NULL);
        new_node = malloc(sizeof(*new_node));
        new_node->symbol= c;
        new_node->amount = 1;
        new_node->next = NULL;
        new_node->passage_0 = NULL;
        new_node->passage_1 = NULL;
        add_to_list(list, new_node);
}

/* Counts symbols in file using a list,
 * int bit determines the ninary length of symbol,
 * looks for the symbol in the list,
 * if it finds it: amount++,
 * if not: add it to the list.
 * returns NULL if it encounters any problems */
heap_t count_symbols (FILE *in, int bit, int VERBOSE) {
        /* depending on bit create symbol using data_t */
        heap_t tmp, list = malloc(sizeof(*list)); /* tmp: first node in list is temporary, we'll need to free it later */
        data_t found_symbol;
        char c, half_char = 0x00;
	rewind(in); /* just in case */
        list->next = NULL;
	if(VERBOSE)
		fprintf(stderr, "COUNTER.C: Starting to count symbols...\n");
        while(c != EOF) {
                /* getting symbol from in FILE */
                if( (c = fgetc(in)) == 0x0A) /* in theory we should read it, but then it's impossible to read per 12 bits */
                        break;
                found_symbol.byte[1] = c;
                if(bit==16) {
                        c = fgetc(in);
                        if(c==EOF) {
                                fprintf(stderr,"ERROR: Given file can't be read per 2 bytes\n");
                                /* !free list */
                                return NULL;
                        }
                        found_symbol.byte[0] = c;
                } else if(bit==12) { /* when reading per 12 bits we encounter the problem of "half byte", so we need to act accordingly */
                        if(half_char != 0x00) {
                                half_byte_transfer(&c, &half_char);
                                found_symbol.byte[1] = half_char;
                                found_symbol.byte[0] = c;
                                half_char = 0x00;
                        } else {
                                c = fgetc(in);
                                found_symbol.byte[0] = (c&0xF0);
                                half_char = (c<<4);
                        }
                } else /* read per 8 bits */
                        found_symbol.byte[0] = 0x00;
                /* adding the found_symbol into the list */
                increase_amount_in_list(list, found_symbol);
        }
        if(half_char!=0x00) {
                fprintf(stderr,"ERROR: Given file can't be read per 12 bits\n");
                /* !free list */
                return NULL;
        }
        tmp = list;
        list = list->next;
        free(tmp);
	if(VERBOSE) {
		fprintf(stderr, "COUNTER.C: Finished counting!\n");
		if(VERBOSE == 3)
			print_list(list);
	}
        return list;
}
