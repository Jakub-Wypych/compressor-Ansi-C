/* Used for storing tools used for compression/decompression */

#include <stdio.h>
#include "compressor.h"
#include "counter.h" /* heap_t */
#include <stdlib.h>


/* Adds bit 0 or 1 on the end of byte (char) */
void add_bit_on_end(char *a, int bit) {
        *a<<=1;
        if(bit)
                *a|=0x01;
}

/* Frees the memory allocated for dictionary_t */
void free_dictionary_t(dictionary_t dictionary) {
        dictionary_t tmp = dictionary;
        while(dictionary != NULL) {
                dictionary = dictionary->next;
                free(tmp);
                tmp = dictionary;
        }
}

/* Prints the contents of dictionary_t */
void read_dictionary(dictionary_t dictionary) {
        dictionary_t tmp = dictionary;
        int counting = 1;
        fprintf(stderr,"\nDICTIONARY\n");
        while(tmp != NULL) {
                fprintf(stderr,"%d. symbol numeric: %d \t\t probability: %d\n", counting++, tmp->symbol.numeric, tmp->probability);
                tmp = tmp->next;
        }
        fprintf(stderr,"\n");
}

void add_repeating_prob(dictionary_t dictionary) {
	int repeats = -1;
	dictionary_t tmp = dictionary, pointer;
	while(tmp->next != NULL) {
		if(tmp->next->probability == tmp->probability) {
			if(repeats == 5) {
				pointer->probability = 15;
				repeats = -1;
			}
			if(repeats == -1)
				pointer = tmp->next;
			repeats++;
		} else {
			if(repeats != -1)
				pointer->probability = 10 + repeats;
			repeats = -1;
		}
		tmp = tmp->next;
	}
	if(repeats != -1)
		pointer->probability = 10 + repeats;
}

/* Creates a struct dicionary_t fom heap_t */
dictionary_t make_dictionary(heap_t list) {
        dictionary_t head = malloc(sizeof(*head)); /* first node is temporary */
        dictionary_t new_node, pos = head;
        heap_t tmp = list;
        do {
                new_node = malloc(sizeof(*new_node));
                new_node->symbol = tmp->symbol;
		new_node->probability = tmp->amount;
                /* attach new node and move to it */
                pos->next = new_node;
                pos = pos->next;
        } while( (tmp = tmp->next) != NULL );
        pos->next = NULL;
        pos = head;
        head = head->next;
        free(pos);
	add_repeating_prob(head);
        return head;
}
