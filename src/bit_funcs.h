/* Used for storing functions used for "bit manipulation" */

#ifndef _BIT_FUNCS_H_IS_INCLUDED_
#define _BIT_FUNCS_H_IS_INCLUDED_

int var_bit_transfer(char *buf, int buf_size, char b, int b_size, FILE *out); /* tries to transfer 8 or les bits into FILE */
int transfer_to_FILE(char *buf, int buf_size, char *transfered_bits, int bit_amount, FILE *out); /* tries to transfer more than 8 bits into FILE */
void half_byte_transfer(char *a, char *b); /* transfers half-byte of 'a' into 'b' */
void write_one_in_pos(char *a, int pos); /* writes "1" in byte in 'pos' */
void showbits(char a, int n); /* reads 'n' bit in byte */

#endif
