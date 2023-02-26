/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#include <stdio.h>
#include <stdlib.h>
#include "csheet.h"
#include "heap.h" /* heap_t */
#include <string.h> /* strcpy */

/* main function for making a csheet from heap */
void cmprs_list(char *code, heap_t pos, csheet_t csheet) {
	if(pos->symbol != '\0') {
		csheet_t new_node = malloc(sizeof(*new_node)); /* !remember to free */
		new_node->symbol = pos->symbol;
		strcpy(new_node->code, code);
		/* !add new_node to end of csheet */
	}
	if(pos->passage_0 != NULL) {
		int depth = 0;
		while(code[depth++]!='\0') {
		/* we are counting how many not NULLs are in code[MAX_CODE] */
		};
		depth++;
		code[depth] = '0';
		cmprs_list(code, pos->passage_0, csheet);
		code[depth] = '1';
		cmprs_list(code, pos->passage_1, csheet);
	}
}

/* Initiallises the process of making csheet from heap */
csheet_t make_cmprs_list(heap_t heap, int VERBOSE) {
	/* !check if heap is one node large and act accordingly */
	csheet_t csheet = malloc(sizeof(*csheet)); /* the first node is only a temporary node */
        /* !also rembember to free */
	char code[MAX_CODE];
	int i;
	for(i=0;i<8;i++)
		code[i]='\0';
	csheet->next = NULL;
        if(VERBOSE)
                fprintf(stderr,"CSHEET.C: Making a compression sheet...\n");
	cmprs_list(code, heap, csheet);
	csheet = csheet->next; /* we remove the temporary node */
	if(VERBOSE)
                fprintf(stderr,"CSHEET.C: Finished making a compression sheet!\n");
	return csheet;
}
