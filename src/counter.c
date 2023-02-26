/* Used for counting how many times a symbol occurs in a file.
Returns a list of symbols and how many times they occured */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"

/* Counts symbols in file using initially an array,
where the index corresponds to the ANSI character code (e.g. 'a' = 96).
Array size is defined by MAX_COUNT, which originally is set to 256.
It then converts said array to list and returns the list */
heap_t count_symbols (FILE *in) {
	int array[MAX_COUNT];
	int i;
	for(i=0;i<MAX_COUNT;i++)
		array[i]=0;
	/* !use "fgetc(in)", conver char into int(x) and "array[x]++;" */
	/* !remember to check if ANSI character code number is larger than MAX_COUNT and act accordingly */
	return array_to_list(array);
}

/* Adds the given node to the end of the given list */
void add_to_list(heap_t list, heap_t node) {
	/* get to the end of list and add node to it */
}

/* Converts array into list */
heap_t array_to_list(int *array) {
	heap_t list = malloc(sizeof(*list)); /* !remeebr to free */
	/* !scan array where array[i]>0,
	create node and assing it a char and int from array[i],
	sets also passage_0 and passage_1 to NULL(we really only need passage_0),
	use add_to_list to add said node to list */
	return list;
}
