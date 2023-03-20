/* Used for converting the list from counter.c into a heap.
 * Returns a heap */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "counter.h" /* heap_t */
#include "list_iterate.h" /* read_heap, free_heap_t */

/* Searches the list for two smallest numbers.
 * If list is one node large, the second number will be returned as a -1 */
void two_min_numbers(heap_t list, int *min_nmbr) {
	heap_t tmp = list->next;
	min_nmbr[0] = list->amount;
	min_nmbr[1] = -1;
	if(list->next == NULL) /* list is one node large */
		return;
	min_nmbr[1] = list->next->amount;
	do {
		if(min_nmbr[0] > tmp->amount) {
			min_nmbr[1] = min_nmbr[0];
			min_nmbr[0] = tmp->amount;
		}
		else if(min_nmbr[1] > tmp->amount)
			min_nmbr[1] = tmp->amount;
	} while( ( tmp = tmp->next ) != NULL );
}

/* Takes and returns targeted node from list.
 * If node is found, it's removed from the list and returned.
 * If node is not found, return NULL */
heap_t take_from_list(heap_t list, int target_nmbr) {
	heap_t target_node = NULL;
	heap_t back = list;
	heap_t front = list->next;
	while(front != NULL && target_node == NULL) {
		if(front->amount==target_nmbr) { /* target in middle */
			target_node = front;
			back->next = front->next;
			front->next=NULL;
			return target_node;
		}
		back = back->next;
		front = front->next;
	}
	if(front->amount == target_nmbr && target_node == NULL) { /* target at the end */
		target_node = front;
		back->next = NULL;
	}
	return target_node;
}

/* Converts list into heap */
heap_t organize_heap(heap_t list, int VERBOSE) {
	int min1, min2, min_nmbr[2];
	heap_t new_head = malloc(sizeof(*new_head));
	new_head->next = list;
        if(VERBOSE)
                fprintf(stderr, "HEAP.C: Beginning the organize heap process...\n");
	two_min_numbers(new_head->next, min_nmbr);
	min1 = min_nmbr[0];
	min2 = min_nmbr[1];
	/* if min2 is equal to 0, that means our list is one node large (exlcluding "new_head") i.e. we finished our heap */
	while(min2 != -1) {
		heap_t passage_0, passage_1, blank_node;
		passage_0 = take_from_list(new_head, min1);
		passage_1 = take_from_list(new_head, min2);
		if(new_head->next == NULL) { /* list was two node large, excluding "new_head" */
			new_head->symbol.numeric = 0;
			new_head->amount = passage_0->amount + passage_1->amount;
			new_head->passage_0 = passage_0;
			new_head->passage_1 = passage_1;
		        if(VERBOSE)
		                fprintf(stderr,"HEAP.C: Finished the organize process!\n");
			return new_head;
		}

		blank_node = malloc(sizeof(*blank_node));
		/* its purpose is to connect two nodes in heap. Visual interpretation:
		blank_node
		   /   \
		  a     b */
		blank_node->symbol.numeric = 0;
		blank_node->amount = passage_0->amount + passage_1->amount;
		blank_node->next = NULL;
		blank_node->passage_0 = passage_0;
		blank_node->passage_1 = passage_1;
		if(VERBOSE == 3)
			fprintf(stderr, "HEAP.C: Made blank node. "
					"Amount: %d. Passage_0: %d. Passage_1: %d\n", blank_node->amount, blank_node->passage_0->symbol.numeric, blank_node->passage_1->symbol.numeric);
		add_to_list(new_head, blank_node);

		two_min_numbers(new_head->next, min_nmbr);
		min1 = min_nmbr[0];
		min2 = min_nmbr[1];
		if(VERBOSE == 3)
                        read_heap(new_head->next);
	}
	free(new_head);
	if(VERBOSE)
		fprintf(stderr,"HEAP.C: Finished the organize process!\n");
	return list;
}
