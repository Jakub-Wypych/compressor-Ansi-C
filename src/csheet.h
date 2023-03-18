/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#ifndef _CSHEET_H_IS_INCLUDED_
#define _CSHEET_H_IS_INCLUDED_

#define MAX_CODE 16 /* defines maximum length of code */

#include "counter.h" /* heap_t */

typedef struct csheet {
	data_t symbol;
	int array_pos;
	int byte_pos;
	char code[MAX_CODE];
	struct csheet *next;
} *csheet_t;

void read_csheet(csheet_t csheet); /* prints csheet */
void cmprs_list (char *code, heap_t pos, csheet_t csheet, int depth); /* main function for making a csheet from heap */
csheet_t make_cmprs_list(heap_t heap, int VERBOSE); /* initiallises the process of making csheet from heap */

#endif
