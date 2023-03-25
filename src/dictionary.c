/* Used for making a dictionary for decompressing/compressing */

#include <stdio.h>
#include "dictionary.h"
#include "counter.h" /* data_t,  heap_t */
#include <stdlib.h>

/* Looks for repeating probabilities in dictionary_t and marks them */
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
