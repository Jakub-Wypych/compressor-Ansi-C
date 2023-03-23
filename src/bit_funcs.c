/* Used for storing functions used for "bit manipulation" */

#include <stdio.h>
#include <stdlib.h>
#include "bit_funcs.h"
#include "counter.h" /* data_t */

/* Used for 8 bit or less writing,
 * buf: byte to which we tansfer bits,
 * buf_size: by how many bits is the buf filled already,
 * b: byte from which we tansfer bits,
 * b_size: by how many bits is the b filled already,
 * out: FILE to which the function will write to.
 * any "stary bits" will be returned in buf,
 * returns the amount of bits left in buf */
int var_bit_transfer(char *buf, int buf_size, char b, int b_size, FILE *out) {
	*buf |= (b>>buf_size);
	if(buf_size+b_size>8) {
		fwrite(buf, 1, 1, out);
		*buf = (b<<(8-buf_size));
	}
	return 8-buf_size; /* buf_size */
}

/* Used for more than one byte writing,
 * buf: byte used to store bits and write them into the out FILE,
 * buf_size: by how many bits is the buf filled already,
 * transfered_bits: array which stores bit for transfer,
 * bit_amount: how many bits to transfer from array transfered_bits,
 * out: FILE to which the function will write to.
 * any "stray bits" will be returned in buf,
 * returns the amount of bits left in buf */
int transfer_to_FILE(char *buf, int buf_size, char *transfered_bits, int bit_amount, FILE *out) {
	int i = 0;
	while(bit_amount/8 > 1) {
		buf_size = var_bit_transfer(buf, buf_size, transfered_bits[i], 8, out);
		i++;
		bit_amount -= 8;
	}
	if(bit_amount != 0)
		buf_size = var_bit_transfer(buf, buf_size, transfered_bits[i], bit_amount, out);
	return buf_size;
}

/* Transfers first 4 bits of a into last 4 bits of b,
 * then shifts last 4 bits in a into first 4 bits in a */
void half_byte_transfer(char *a, char *b) { /* char b is "only 4 bits large" */
        *b |= (((*a)&=0xF0)>>4);
        *a<<=4;
}

/* Writes bit '1' in defined pos,
 * where pos = < 0; 7 > */
void write_one_in_pos(char *a, int pos) {
	unsigned char mask = 0x80;
	mask >>= pos;
	*a |= mask;
}

/* Prints in stderr 'n' bits in byte */
void showbits(char a, int n) {
	unsigned char mask = 0x80;
	int i;
	for(i=1;i<=n;i++) {
		if(a&mask)
			fprintf(stderr,"1");
		else
			fprintf(stderr,"0");
		mask>>=1;
	}
}

/* Creates bitwork */
bit_work_t init_bitwork(FILE *file, unsigned char password) {
	bit_work_t bitwork = malloc(sizeof(*bitwork));
	bitwork->buf.all = 0;
	bitwork->buf_size = 0;
	rewind(file);
	bitwork->file = file;
	bitwork->password = password;
	return bitwork;
}

/* Reads <1; 16> amount of bits from FILE and returns how many bits it read,
 * data: read bits will be returned in data_t,
 * bit_amount: how many bits to read from FILE,
 * bitwork: stores leftover bits as well as the FILE */
int fbit_read(data_t *data, int bit_amount, bit_work_t bitwork) {
	int amount_read = bit_amount;
	char read_byte;
	unsigned char mask = 0xFF;
	int shift[2] = { 0, 0 };
	if(bit_amount>=8) {
		shift[0] = 8;
		shift[1] = bit_amount-8;
	} else
		shift[0] = bit_amount;
	data->numeric = 0;
	while(bitwork->buf_size<bit_amount) {
		if(fread(&read_byte, 1, 1, bitwork->file) == 0) {
			amount_read = bitwork->buf_size;
			break;
		}
		bitwork->buf.all<<=8;
		bitwork->buf.byte.four = read_byte;
		bitwork->buf_size += 8;
	}
	bitwork->buf.all <<= (32-bitwork->buf_size);
	data->byte[1] = (bitwork->buf.byte.one&(mask<<(8-shift[0])));
	data->byte[0] = (bitwork->buf.byte.two&(mask<<(8-shift[1])));
	bitwork->buf.all <<= bit_amount;
	bitwork->buf_size -= bit_amount;
	bitwork->buf.all >>= (32-bitwork->buf_size);
	return amount_read;
}
