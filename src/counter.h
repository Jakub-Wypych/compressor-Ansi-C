/* Used for counting how many times a symbol occurs in a file.
 * Returns a list of symbols and how many times they occured */

#ifndef _COUNTER_H_IS_INCLUDED_
#define _COUNTER_H_IS_INCLUDED_

/* This structure has a dual purpose:
 * 1. It can be used as a list which stores a symbol (char) and its amount (int)
 * 2. Can be used/converted to create a heap */
typedef union {
        unsigned short int numeric; /* symbol stored in numeric form */
        unsigned char byte[2]; /* [1] is first byte, [0] is last byte */
} data_t;

typedef struct heap {
        data_t symbol;
        int amount; /* e.g. 27 */
        struct heap *next; /* list (1.) */
        struct heap *passage_0; /* heap (2.) */
        struct heap *passage_1; /* heap (2.) */
} *heap_t;

void increase_amount_in_list(heap_t list, data_t c); /* increases the amount of char in list */
heap_t count_symbols (FILE *in, int bit, int VERBOSE); /* counts symbols in FILE */

#endif
