/* Used for storing functions for converting counted list of symbols */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h"
#include "heap.h" /* take_from_list */

/* Calculates ten_to_the_power_of for amount_to_probability */
int calculate_to_what_power(heap_t list, int amount_of_all_symbols) {
        int power = 0;
        double largest_amount = 0;
        heap_t tmp = list;
        while(tmp->next != NULL)
                tmp = tmp->next;
        largest_amount = tmp->amount;
        largest_amount = largest_amount/amount_of_all_symbols;
        while(largest_amount<1) {
                largest_amount = largest_amount*10;
                power++;
        };
        return power;
}

/* Changes amount into probability rounded to tenths and multiplies by 10 */
void amount_to_probability(int *amount, double amount_of_all_symbols, int ten_to_the_power_of) {
        double tmp = *amount;
        int i;
        tmp = tmp/amount_of_all_symbols;
        for(i=0;i<ten_to_the_power_of;i++)
                tmp = tmp*10;
        *amount = tmp;
}

/* Finds the smallest number in list */
int one_min_number(heap_t list) {
        heap_t tmp = list;
        int min = list->amount;
        while( ( tmp = tmp->next) != NULL)
                if(min > tmp->amount) {
                        min = tmp->amount;
        }
        return min;
}

/* Sorts given list  */
void sort_list(heap_t list) {
        int min;
        heap_t new_node, tmp;
        heap_t sorted_list = malloc(sizeof(*sorted_list));
        sorted_list->next = NULL;
        while(list->next!=NULL) {
                min = one_min_number(list->next);
                new_node = take_from_list(list, min);
                add_to_list(sorted_list, new_node);
        }
        tmp = sorted_list;
        sorted_list = sorted_list->next;
        free(tmp);
        list->next = sorted_list;
}
