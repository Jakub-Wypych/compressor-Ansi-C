/* Used for printing and clearing lists */

#include <stdio.h>
#include <stdlib.h>
#include "bit_funcs.h"
#include "counter.h" /* heap_t */
#include "csheet.h" /* csheet_t */
#include "dictionary.h" /* dictionary_t */

/* Prints in stderr heap_t list */
void read_heap (heap_t list) {
        heap_t tmp = list;
        int i = 1;
        fprintf(stderr, "\nLIST\n");
        do {
                fprintf(stderr, "Node %d: %d %d\n", i++, tmp->symbol.numeric, tmp->amount);
        } while( (tmp=tmp->next) != NULL );
        fprintf(stderr, "\n");
}

/* Frees the memory allocated for struct heap_t */
void free_heap_t(heap_t heap) {
	heap_t tmp = heap;
	while(heap!=NULL) {
		heap = heap->next;
		free(tmp);
		tmp = heap;
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
			showbits(tmp->code[j], 8);
		}
		showbits(tmp->code[j], tmp->byte_pos);
	} while( (tmp = tmp->next) != NULL);
	fprintf(stderr,"\n\n");
}

/* Frees the allocated memory for structure csheet_t */
void free_csheet_t(csheet_t csheet) {
	csheet_t tmp = csheet;
        while(csheet!=NULL) {
                csheet = csheet->next;
                free(tmp);
                tmp = csheet;
        }
}

/* Prints the contents of dictionary_t */
void read_dictionary(dictionary_t dictionary) {
        dictionary_t tmp = dictionary;
        int counting = 1;
        fprintf(stderr,"\nDICTIONARY\n");
        while(tmp != NULL) {
                fprintf(stderr,"%d. symbol numeric: %d \t\t probability: %d\n", counting++, tmp->symbol.numeric, tmp->probability);
                tmp = tmp->next;
        }
        fprintf(stderr,"\n");
}

/* Frees the memory allocated for dictionary_t */
void free_dictionary_t(dictionary_t dictionary) {
        dictionary_t tmp = dictionary;
        while(dictionary != NULL) {
                dictionary = dictionary->next;
                free(tmp);
                tmp = dictionary;
        }
}
