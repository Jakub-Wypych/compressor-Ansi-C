/* Used for converting the list from counter.c into a heap.
 * Returns a heap */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "counter.h" /* add_to_list, heap_t */

/* Searches the list for two smallest numbers.
 * If list is one node large, the second number will be returned as a 0 */
void two_min_numbers(heap_t list, int *min_nmbr) {
	heap_t tmp = list->next;
	/* adding value to min_nmber[0] */
	min_nmbr[0] = list->amount;
	min_nmbr[1] = 0;
	if(list->next == NULL) /* list is one node large */
		return;
	min_nmbr[1] = list->next->amount;
	while(tmp != NULL) {
		if(min_nmbr[0] >= tmp->amount) {
			min_nmbr[1] = min_nmbr[0];
			min_nmbr[0] = tmp->amount;
		}
		else if(min_nmbr[1] > tmp->amount)
			min_nmbr[1] = tmp->amount;
		tmp = tmp->next;
	}
}

/* Takes and returns targeted node from list.
 * If node is found, it's removed from the list and returned.
 * If node is not found, return NULL */
heap_t take_from_list(heap_t list, int target_nmbr) {
	heap_t target_node = NULL;
	heap_t back = list;
	heap_t front = list->next;
	if(list->amount == target_nmbr) { /* target on start */
		target_node = list; /* we can't remove the node from start,
				       we'll have to do it outside of this function */
	}
	while(front != NULL && target_node == NULL) {
		if(front->amount==target_nmbr) { /* target in middle */
			target_node = front;
			back->next = front->next;
			break;
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
	int min1, min2;
	int list_is_two_node_large = 0;
	int min_nmbr[2];
        if(VERBOSE)
                fprintf(stderr, "HEAP.C: Beginning the organize heap process...\n");
	two_min_numbers(list, min_nmbr);
	min1 = min_nmbr[0];
	min2 = min_nmbr[1];
	/* if min2 is equal to 0, that means our list is one node large a.k.a. we finished our heap */
	while(min2 != 0) {
		heap_t passage_0, passage_1, blank_node;
		if(list->next->next == NULL) { /* list is two node large */
			list_is_two_node_large = 1;
			passage_0 = list;
			passage_1 = list->next;
		}
		else {
			passage_0 = take_from_list(list, min1);
			if(list == passage_0) /* checking if the node was on the start */
				list = list->next;
			passage_1 = take_from_list(list, min2);
			if(list == passage_1)
				list = list->next;
		}
		blank_node = malloc(sizeof(*blank_node));
		/* its purpose is to connect two nodes in heap. Visual interpretation:
		blank_node
		   /   \
		  a     b */
		blank_node->symbol = '\0';
		blank_node->amount = passage_0->amount + passage_1->amount;
		blank_node->next = NULL;
		blank_node->passage_0 = passage_0;
		blank_node->passage_1 = passage_1;
		if(VERBOSE)
			fprintf(stderr, "HEAP.C: Made blank node. "
					"Amount: %d. Passage_0: %d. Passage_1: %d\n", blank_node->amount, blank_node->passage_0->symbol, blank_node->passage_1->symbol);
		add_to_list(list, blank_node);

		if(list_is_two_node_large) {
			list = list->next->next;
			if(VERBOSE)
				print_list(list);
			break;
		}
		two_min_numbers(list, min_nmbr);
		min1 = min_nmbr[0];
		min2 = min_nmbr[1];
		if(VERBOSE)
                        print_list(list);
	}
	if(VERBOSE)
		fprintf(stderr,"HEAP.C: Finished the organize process!\n");
	return list;
}
