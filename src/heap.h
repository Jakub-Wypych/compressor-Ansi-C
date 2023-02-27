/* Used for converting the list from counter.c into a heap.
 * Returns a heap */

#ifndef _HEAP_H_IS_INCLUDED_
#define _HEAP_H_IS_INCLUDED_

#include "counter.h" /* add_to_list, heap_t */

void two_min_numbers(heap_t list, int *min_nmbr); /* searches the list for two smallest numbers */
heap_t take_from_list(heap_t list, int target); /* takes and returns targeted node from list */
heap_t organize_heap(heap_t list, int VERBOSE); /* converts list into heap */

#endif
