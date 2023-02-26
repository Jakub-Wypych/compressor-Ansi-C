/* Used for counting how many times a symbol occurs in a file.
Returns a list of symbols and how many times they occured */

#ifndef _COUNTER_H_IS_INCLUDED_
#define _COUNTER_H_IS_INCLUDED_

#define MAX_COUNT 255

/* This structure has a dual purpose:
1. It can be used as a list which stores a symbol (char) and its amount (int)
2. Can be used/converted to create a heap */
typedef struct heap {
	char symbol; /* e.g. 'a' */
	int amount; /* e.g. 27 */
	struct heap *next; /* list (1.) */
	struct heap *passage_0; /* heap (2.) */
	struct heap *passage_1; /* heap (2.) */
} *heap_t;

heap_t count_symbols(FILE *in); /* counts symbols in FILE */
void add_to_list(heap_t list, heap_t node); /* adds node to the end of list */
heap_t array_to_list(int *array); /* converts array into list */

#endif
