/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#include <stdio.h>
#include <stdlib.h>
#include "csheet.h"
#include "heap.h" /* heap_t */
#include <string.h> /* strcpy */

/* main function for making a csheet from heap */
void cmprs_list(char *code, heap_t pos, csheet_t csheet) {
	if(pos->symbol != '\0') {
		csheet_t tmp = csheet;
		csheet_t new_node = malloc(sizeof(*new_node));
		new_node->next = NULL;
		new_node->symbol = pos->symbol;
		strcpy(new_node->code, code);
		while(tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = new_node;
	}
	if(pos->passage_0 != NULL) {
		int depth = 0;
		while(code[depth]!='2') {
			depth++;
		/* we are counting how many not NULLs are in code[MAX_CODE] */
		}
		if(depth > MAX_CODE) {
			fprintf(stderr, "ERROR: MAX_CODE IS TO SMALL!\n");
			return;
		}
		code[depth] = '0';
		cmprs_list(code, pos->passage_0, csheet);
		code[depth] = '1';
		cmprs_list(code, pos->passage_1, csheet);
		/*free(pos); "should" work, !remove comment later*/
	}
}

/* Initiallises the process of making csheet from heap */
csheet_t make_cmprs_list(heap_t heap, int VERBOSE) {
	csheet_t csheet = malloc(sizeof(*csheet)); /* the first node is only a temporary node */
	char code[MAX_CODE];
	int i;
	for(i=0;i<MAX_CODE;i++)
		code[i]='2';
	csheet->next = NULL;
        if(VERBOSE)
                fprintf(stderr,"CSHEET.C: Making a compression sheet...\n");
	if(heap->passage_0 == NULL) { /* heap is one node large */
	        csheet->symbol = heap->symbol;
		code[0]='0';
                strcpy(csheet->code, code);
	        if(VERBOSE)
        	        fprintf(stderr,"CSHEET.C: Finished making a one node large compression sheet!\n");
        	return csheet;

        }
	cmprs_list(code, heap, csheet);
	csheet = csheet->next; /* we remove the temporary node */
	if(VERBOSE)
                fprintf(stderr,"CSHEET.C: Finished making a compression sheet!\n");
	return csheet;
}
