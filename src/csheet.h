/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#ifndef _CSHEET_H_IS_INCLUDED_
#define _CSHEET_H_IS_INCLUDED_

#define MAX_CODE 16 /* defines maximum length of code */

#include "heap.h" /* heap_t */

typedef struct csheet {
	char symbol;
	char code[MAX_CODE];
	struct csheet *next;
} *csheet_t;

void cmprs_list (char *code, heap_t pos, csheet_t csheet); /* main function for making a csheet from heap */
csheet_t make_cmprs_list(heap_t heap, int VERBOSE); /* initiallises the process of making csheet from heap */

#endif
