/* Used for making a dictionary for decompressing/compressing */

#ifndef _DICTIONARY_H_IS_INCLUDED_
#define _DICTIONARY_H_IS_INCLUDED_

#include "counter.h" /* data_t, heap_t */

typedef struct dictionary {
        data_t symbol;
        unsigned char probability; /* <0;15> */
        struct dictionary *next;
} *dictionary_t;

dictionary_t make_dictionary(heap_t list); /* creates a dictionary from heap_t */

#endif

