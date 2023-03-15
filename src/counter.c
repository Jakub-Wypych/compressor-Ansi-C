/* Used for counting how many times a symbol occurs in a file.
 * Returns a list of symbols and how many times they occured */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"
#include "utility.h"
#include "heap.h" /* take_from_list */

/* Calculates ten_to_the_power_of for amount_to_probability */
int calculate_to_what_power(heap_t list, int amount_of_all_symbols) {
	int power = 0;
	double largest_amount = 0;
	heap_t tmp = list;
	while(tmp->next != NULL)
		tmp = tmp->next;
	largest_amount = tmp->amount;
	largest_amount = largest_amount/amount_of_all_symbols;
	while(largest_amount<1) {
		largest_amount = largest_amount*10;
		power++;
	};
	return power;
}

/* Changes amount into probability rounded to tenths and multiplies by 10 */
void amount_to_probability(int *amount, double amount_of_all_symbols, int ten_to_the_power_of) {
	double tmp = *amount;
	int i;
	tmp = tmp/amount_of_all_symbols;
	for(i=0;i<ten_to_the_power_of;i++)
		tmp = tmp*10;
	*amount = tmp;
}

/* Finds the smallest number in list */
int one_min_number(heap_t list) {
	heap_t tmp = list;
	int min = list->amount;
	while( ( tmp = tmp->next) != NULL)
		if(min > tmp->amount) {
			min = tmp->amount;
	}
	return min;
}

/* Sorts given list  */
void sort_list(heap_t list) {
	int min;
	heap_t new_node, tmp; 
	heap_t sorted_list = malloc(sizeof(*sorted_list));
	sorted_list->next = NULL;
	while(list->next!=NULL) {
		min = one_min_number(list->next);
		new_node = take_from_list(list, min);
		add_to_list(sorted_list, new_node);
	}
	tmp = sorted_list;
	sorted_list = sorted_list->next;
	free(tmp);
	list->next = sorted_list;
}

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
	size_t amount_read;
        char c, half_char = 0x00; /* for 12 bit reading */
	rewind(in); /* just in case */
        list->next = NULL;
	if(VERBOSE)
		fprintf(stderr, "COUNTER.C: Starting to count symbols...\n");
        amount_read = fread(&c,1,1,in);
	while(amount_read != 0) {
                /* getting symbol from in FILE */
		amount_of_all_symbols++;
                found_symbol.byte[1] = c;
                if(bit==16) {
                        amount_read = fread(&c,1,1,in);
                        if(amount_read == 0) {
                                fprintf(stderr,"ERROR: Given file can't be read per 2 bytes; missing 1 byte\n");
                                free_heap_t(list);
                                return NULL;
                        }
			amount_of_all_symbols++;
                        found_symbol.byte[0] = c;
                } else if(bit==12) { /* when reading per 12 bits we encounter the problem of "half byte", so we need to act accordingly */
                        if(half_char != 0x00) {
                                half_byte_transfer(&c, &half_char);
                                found_symbol.byte[1] = half_char;
                                found_symbol.byte[0] = c;
                                half_char = 0x00;
                        } else {
                                amount_read = fread(&c,1,1,in);
				if(amount_read == 0) {
                                	fprintf(stderr,"ERROR: Given file can't be read per 12 bits; missing 2 bytes\n");
                                	free_heap_t(list);
                                	return NULL;
                        	}
				amount_of_all_symbols++;
                                found_symbol.byte[0] = (c&0xF0);
                                half_char = (c<<4);
                        }
                } else /* read per 8 bits */
                        found_symbol.byte[0] = 0x00;
                /* adding the found_symbol into the list */
                increase_amount_in_list(list, found_symbol);
		amount_read = fread(&c,1,1,in);
        }
        if(half_char!=0x00) {
                fprintf(stderr,"ERROR: Given file can't be read per 12 bits; missing 1 byte\n");
                free_heap_t(list);
                return NULL;
        }
	if(VERBOSE>1) {
		fprintf(stderr, "COUNTER.C: Unsorted list:"); print_list(list->next); }
	sort_list(list);
        tmp = list;
        list = list->next;
        free(tmp);
	tmp = list;
	if(VERBOSE>1) {
		fprintf(stderr, "COUNTER.C: Sorted list:"); print_list(list); }
	/* converting amount into probability */
	ten_to_the_power_of = calculate_to_what_power(list, amount_of_all_symbols);
	do {
		amount_to_probability(&tmp->amount, amount_of_all_symbols, ten_to_the_power_of);
	} while( (tmp = tmp->next) != NULL);
	if(VERBOSE) {
		fprintf(stderr, "COUNTER.C: Finished counting!\n");
		if(VERBOSE == 3)
			print_list(list);
	}
        return list;
}
