/* Used for printing and clearing lists */
#ifndef _LIST_ITERATE_H_IS_INCLUDED_
#define _LIST_ITERATE_H_IS_INCLUDED_

#include <stdio.h>
#include "counter.h" /* heap_t */
#include "csheet.h" /* csheet_t */
#include "compressor.h" /* dictionary_t */

void read_heap(heap_t list);  /* prints heap */
void free_heap_t(heap_t heap); /* frees heap_t */
void read_csheet(csheet_t csheet); /* prints csheet */
void free_csheet_t(csheet_t csheet); /* frees csheet_t */
void read_dictionary(dictionary_t dictionary); /* prints dictionary */
void free_dictionary_t(dictionary_t dictionary); /* frees dictionary */

#endif
