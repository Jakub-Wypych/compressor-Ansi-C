/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#include <stdio.h>
#include <stdlib.h>
#include "csheet.h"
#include "heap.h" /* heap_t */
#include <string.h> /* strcpy */

/* Writes bit '1' in defined pos,
 * where pos = < 0; 7 > */
void write_one_in_pos(char *a, int pos) {
	unsigned char mask = 0x80;
	mask >>= pos;
	*a |= mask;
}

/* reads bits in byte */
void showbits(char a, int n){
	unsigned char mask = 0x80;
	int i;
	for(i=0;i<=n;i++) {
		if(a&mask)
			fprintf(stderr,"1");
		else
			fprintf(stderr,"0");
		mask>>=1;
	}
}

/* Prints in stderr the contents of csheet_t */
void read_csheet(csheet_t csheet) {
	csheet_t tmp = csheet;
	int counting = 1;
        fprintf(stderr,"\nCSHEET");
        do {
		int j = 0;
		fprintf(stderr, "\n%d. symbol numeric: %d \t\t code: ", counting++, tmp->symbol.numeric);
		for(j=0;j != tmp->array_pos;j++) {
			showbits(tmp->code[j], 7);
		}
		showbits(tmp->code[j], tmp->byte_pos);
	} while( (tmp = tmp->next) != NULL);
	fprintf(stderr,"\n\n");
}

/* main function for making a csheet from heap */
void cmprs_list(char *code, heap_t pos, csheet_t csheet, int depth) {
	char code_copy[MAX_CODE];
	int array_pos =0, byte_pos = depth, i;
	for(i=0;i<MAX_CODE;i++)
		code_copy[i] = code[i];
        while(byte_pos>7) {
                byte_pos -=8;
                array_pos++;
        }
	if(pos->symbol.numeric != 0) {
		csheet_t tmp = csheet;
		csheet_t new_node = malloc(sizeof(*new_node));
		new_node->next = NULL;
		new_node->symbol = pos->symbol;
		new_node->array_pos = array_pos;
		new_node->byte_pos = byte_pos-1;
		strcpy(new_node->code, code_copy);
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
		free(pos);
		return;
	}
	cmprs_list(code_copy, pos->passage_0, csheet, depth+1);
	write_one_in_pos(&code_copy[array_pos], byte_pos);
	cmprs_list(code_copy, pos->passage_1, csheet, depth+1);
	free(pos);
}

/* Initiallises the process of making csheet from heap */
csheet_t make_cmprs_list(heap_t heap, int VERBOSE) {
	csheet_t csheet = malloc(sizeof(*csheet)); /* the first node is only a temporary node */
	csheet_t delete_me = csheet;
	char code[MAX_CODE];
	int i;
	for(i=0;i<MAX_CODE;i++)
		code[i]=0x00;
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
	cmprs_list(code, heap, csheet, 0);
	csheet = csheet->next; /* we remove the temporary node */
	free(delete_me);
	if(VERBOSE)
                fprintf(stderr,"CSHEET.C: Finished making a compression sheet!\n");
	return csheet;
}
