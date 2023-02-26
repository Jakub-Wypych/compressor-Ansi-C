/* Used for converting the list from counter.c into a heap.
Returns a heap */

#include <stdio.h>
#include <heap.h>
#include <counter.h> /* add_to_list, heap_t */

/* Searches the list for two smallest numbers.
If list is one node large, the second number will be returned as a 0 */
int *2_min_numbers(heap_t list) {
	int min_nmbr[2];
	int min_nmbr[0] = list->amount;
	int min_nmbr[1] = 0;
	if(list->next == NULL) /* list is one node large */
		return min_nmbr;
	/* !scan the list and write two smallest numbers into int min_nmbr[2] */
	return min_nmbr;
}

/* Takes and returns targeted node from list.
If node is found, it's removed from the list and returned.
If node is not found, return NULL */
heap_t take_from_list(heap_t list, int target_nmbr) {
	heap_t target_node = NULL;
	/* !scan list for target_nmbr */
	/* !if found node is at the start of the list, act accordingly */
	/* !if found node is at the end of the list, act accordingly */
	/* !if found node is in the middle of the list, act accordingly */
	/* !if node is not found in the list, act accordingly */
	return target_node;
}

/* Converts list into heap */
void organize_heap(heap_t list) {
	int min1, min2;
	int *min_nmbr = 2_min_numbers(list);
	min1 = min_nmbr[0];
	min2 = min_nmbr[1];
	/* if min2 is equal to 0, that means our list is one node large a.k.a. we finished our heap */
	while(min2 != 0) {
		heap_t passage_0 = take_from_list(list, min1);
		heap_t passage_1 = take_from_list(list, min2);
		heap_t blank_node;
		/* its purpose is to connect two nodes in heap. Visual interpretation:
		blank_node
		   /   \
		  a     b */
		
		blank_node->symbol = NULL;
		blank_node->amount = min1+min2;
		blank_node->pasage_0 = passage_0;
		blank_node->pasage_1 = passage_1;
		add_to_list(list, blank_node);
		
		min_nmbr = 2_min_numbers(list);
		min1 = min_nmbr[0];
		min2 = min_nmbr[1];
	}
}
