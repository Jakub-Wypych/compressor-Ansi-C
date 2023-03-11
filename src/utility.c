/* Used for storing functions that have versitile use */

#include <stdio.h>
#include "utility.h"
#include "counter.h" /* heap_t */

/* Prints in stderr heap_t list */
void print_list (heap_t list) {
        heap_t tmp = list;
        int i = 1;
        fprintf(stderr, "\nLIST\n");
        do {
                fprintf(stderr, "Node %d: %d %d\n", i++, tmp->symbol.numeric, tmp->amount);
        } while( (tmp=tmp->next) != NULL );
        fprintf(stderr, "\n");
}

/* Adds the given node to the end of the given list */
void add_to_list(heap_t list, heap_t node) {
        heap_t tmp = list;
        while( tmp->next != NULL)
                tmp = tmp->next;
        tmp->next=node;
}

/* transfers first 4 bits of a into last 4 bits of b,
 * then shifts last 4 bits in a into first 4 bits in a */
void half_byte_transfer(char *a, char *b) { /* char b is "only 4 bits large" */
        *b |= (((*a)&=0xF0)>>4);
        *a<<=4;
}
