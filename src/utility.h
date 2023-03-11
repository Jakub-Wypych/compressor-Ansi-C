/* Used for storing functions that have versitile use */
#ifndef _UTILITY_H_IS_INCLUDED_
#define _UTILITY_H_IS_INCLUDED_

#include <stdio.h>
#include "counter.h" /* heap_t */

void print_list(heap_t list);  /* prints list in stderr */
void add_to_list(heap_t list, heap_t node); /* adds node to the end of list */
void half_byte_transfer(char *a, char *b); /* transfers half-byte of a into b */
#endif
