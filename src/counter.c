/* Used for counting how many times a symbol occurs in a file.
 * Returns a list of symbols and how many times they occured */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"
#include "bit_funcs.h" /* fbit_read, bit_work_t */
#include "list_iterate.h" /* read_heap, free_heap_t */

/* Adds the given node to the end of the given list */
void add_to_list(heap_t list, heap_t node) {
        heap_t tmp = list;
        while( tmp->next != NULL)
                tmp = tmp->next;
        tmp->next=node;
}

/* Increases the amount of data_t in list */
void increase_amount_in_list(heap_t list, data_t c) {
        heap_t new_node, tmp = list;
        do {
                if(c.numeric == tmp->symbol.numeric) {
                        tmp->amount++;
                        return;
                }
        } while( (tmp = tmp->next) != NULL);
        new_node = malloc(sizeof(*new_node));
        new_node->symbol = c;
        new_node->amount = 1;
        new_node->next = NULL;
        new_node->passage_0 = NULL;
        new_node->passage_1 = NULL;
        add_to_list(list, new_node);
}

/* Counts symbols in file using a list,
 * int bit determines the binary length of symbol,
 * looks for the symbol in the list,
 * if it finds it: amount++,
 * if not: add it to the list.
 * returns NULL if it encounters any problems */
heap_t count_symbols (FILE *in, int bit, int VERBOSE) {
	int amount_of_all_symbols = 0, ten_to_the_power_of = 1;
        /* depending on bit create symbol using data_t */
        heap_t tmp, list = malloc(sizeof(*list)); /* tmp: first node in list is temporary, we'll need to free it later */
        data_t found_symbol;
	bit_work_t bitread = init_bitwork(in, 0x00);
        list->next = NULL;
	list->symbol.numeric = 0;
	list->amount = 0;
	if(VERBOSE)
		fprintf(stderr, "COUNTER.C: Starting to count symbols...\n");
	/* checking if file can be read per 12 or 16 bits */
	fseek(in, 0, SEEK_END);
	if((bit == 12 && ftell(in)%3 != 0) || (bit == 16 && ftell(in)%2 != 0)) {
		fprintf(stderr, "ERROR: Cannot read per %d bit!\n", bit);
		free(list);
		free(bitread);
		return NULL;
	}
	/* finding symbols in FILE */
	rewind(in);
	while(fbit_read(&found_symbol, bit, bitread) == bit) {
		amount_of_all_symbols++;
		increase_amount_in_list(list, found_symbol);
	}
	free(bitread);
	if(VERBOSE>1) {
		fprintf(stderr, "COUNTER.C: Unsorted list:"); read_heap(list->next); }
	sort_list(list);
	if(list->amount == 0) { /* There wasn't a NULL symbol in file, we're freeing the first node in list */
        	tmp = list;
        	list = list->next;
        	free(tmp);
	}
	tmp = list;
	if(VERBOSE>1) {
		fprintf(stderr, "COUNTER.C: Sorted list:"); read_heap(list); }
	/* converting amount into probability */
	ten_to_the_power_of = calculate_to_what_power(list, amount_of_all_symbols);
	do {
		amount_to_probability(&tmp->amount, amount_of_all_symbols, ten_to_the_power_of);
	} while( (tmp = tmp->next) != NULL);
	if(VERBOSE) {
		fprintf(stderr, "COUNTER.C: Finished counting!\n");
		if(VERBOSE == 3)
			read_heap(list);
	}
        return list;
}
