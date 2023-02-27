/* Used for counting how many times a symbol occurs in a file.
 * Returns a list of symbols and how many times they occured */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"

/* Counts symbols in file using a list,
 * looks for the symbol in the list,
 * if it finds it: amount++,
 * if not: add it to the list. */
heap_t count_symbols (FILE *in, int VERBOSE) {
	char c;
	heap_t tmp; /* the first node in list is temporary */
	heap_t list = malloc(sizeof(*list));
	list->next = NULL;
	if(VERBOSE)
		fprintf(stderr, "COUNTER.C: Starting to count symbols...\n");
	while( (c = fgetc(in)) != EOF ) {
		increase_amount_in_list(list, c);
	}
	tmp = list;
        list = list->next;
        free(tmp);
	if(VERBOSE) {
		fprintf(stderr, "COUNTER.C: Finished counting!\n");
		print_list(list);
	}
	return list;
}

/* Adds the given node to the end of the given list */
void add_to_list(heap_t list, heap_t node) {
	heap_t tmp = list;
	while( tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next=node;
}

/* Increases the amount of symbol in list */
void increase_amount_in_list(heap_t list, char c) {
	heap_t tmp = list;
	heap_t new_node;
	while( tmp != NULL) {
		if(c == tmp->symbol) {
			tmp->amount++;
			return;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(*new_node));
        new_node->symbol= c;
        new_node->amount = 1;
        new_node->next = NULL;
        new_node->passage_0 = NULL;
        new_node->passage_1 = NULL;
        add_to_list(list, new_node);
}

/* Prints in stderr heap_t list */
void print_list (heap_t list) {
        heap_t tmp = list;
        int i = 1;
        fprintf(stderr, "\nLIST\n");
        while(tmp != NULL) {
                fprintf(stderr, "Node %d: %d %d\n", i, tmp->symbol, tmp->amount);
                tmp = tmp->next;
                i++;
        }
        fprintf(stderr, "\n");
}

