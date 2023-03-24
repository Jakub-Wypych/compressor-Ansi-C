/* Used for storing functions used for "bit manipulation" */

#ifndef _BIT_FUNCS_H_IS_INCLUDED_
#define _BIT_FUNCS_H_IS_INCLUDED_

#include "counter.h" /* data_t */

typedef struct {
	char four;
	char three;
	char two;
	char one;
} byte_t;


typedef union {
	int all;
	byte_t byte;
} buf_t;

typedef struct {
	buf_t buf;
	char buf_size; /* how many left overbits are in buf */
	FILE *file; /* FILE which the struct will be working with */
	unsigned char password; /* password which bitwork will be using */
} *bit_work_t;

int var_bit_transfer(char *buf, int buf_size, char b, int b_size, FILE *out); /* tries to transfer 8 or les bits into FILE */
int transfer_to_FILE(char *buf, int buf_size, char *transfered_bits, int bit_amount, FILE *out); /* tries to transfer more than 8 bits into FILE */
void half_byte_transfer(char *a, char *b); /* transfers half-byte of 'a' into 'b' */
void write_one_in_pos(char *a, int pos); /* writes "1" in byte in 'pos' */
void showbits(char a, int n); /* reads 'n' bit in byte */
bit_work_t init_bitwork(FILE *file, unsigned char password); /* needed for bit reading */
int fbit_read(data_t *data, int bit_amount, bit_work_t bitwork); /* reads <1; 16> bits */
void fbit_write(char *transf, int bit_amount, bit_work_t bitwork); /* writes bits into FILE */

#endif
