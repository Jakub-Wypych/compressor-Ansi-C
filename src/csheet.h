/* Used for making the "compression sheet" (e.g. 'a' is 010) */

#ifndef _CSHEET_H_IS_INCLUDED_
#define _CSHEET_H_IS_INCLUDED_

#define MAX_CODE 8 /* defines maximum length of code */

typedef struct csheet {
   char symbol;
   char code[8];
   csheet->next;
} *csheet_t

void cmprs_list (char *code, heap_t pos, csheet_t csheet); /* main function for making a csheet from heap */
csheet_t make_cmprs_list(heap_t heap); /* initiallises the process of making csheet from heap */

#endif
