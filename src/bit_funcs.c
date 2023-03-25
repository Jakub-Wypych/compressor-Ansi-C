/* Used for storing functions used for "bit manipulation" */

#include <stdio.h>
#include <stdlib.h>
#include "bit_funcs.h"
#include "counter.h" /* data_t */

/* Writes bit '1' in defined pos, where pos = < 0; 7 > */
void write_one_in_pos(char *a, int pos) {
	unsigned char mask = 0x80;
	mask >>= pos;
	*a |= mask;
}

/* Prints in stderr <1;8> bits in byte */
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
	rewind(file); /* !questionable */
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
	/* Creating masks for data->byte */
	unsigned char mask = 0xFF;
	int shift[2] = { 0, 0 };
	if(bit_amount>=8) {
		shift[0] = 8;
		shift[1] = bit_amount-8;
	} else
		shift[0] = bit_amount;
	data->numeric = 0;
	while(bitwork->buf_size<bit_amount) {
		if(fread(&read_byte, 1, 1, bitwork->file) == 0) { /* we couldn't read the asked amount of bits */
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
	bitwork->buf.all >>= (32-bitwork->buf_size);
	bitwork->buf_size -= bit_amount;
	return amount_read;
}

/* Writes 'bit_amount' of bits from '*transf' into bitwork->file,
 * *transf: stores the bits to be written into file,
 * bit_amount: how many bits to transfer from *transf,
 * bitwork: stores the leftover bits as well as the FILE. */
void fbit_write(char *transf, int bit_amount, bit_work_t bitwork) {
	int i = 0;
	while(bit_amount/8>=1) {
		/* writing 8 bits into file */
		bitwork->buf.all<<=8;
		bitwork->buf.byte.four = transf[i++];
		bitwork->buf.all<<=(32-(8+bitwork->buf_size));
		fwrite(&bitwork->buf.byte.one, 1, 1, bitwork->file);
		bitwork->buf.all>>=(32-(8+bitwork->buf_size));
		bit_amount -= 8;
	}
	if(bit_amount==0) /* we've wrote all that was needed (bit_amount is dividable by 8 */
		return;
	/* we add leftoverbits to bitwork */
	bitwork->buf.all<<=8;
	bitwork->buf.byte.four = transf[i];
	bitwork->buf.all>>=(8-bit_amount);
	bitwork->buf_size += bit_amount;
	if(bitwork->buf_size<8) /* we can't write the leftover bits */
		return;
	bitwork->buf.all<<=(32-bitwork->buf_size);
        fwrite(&bitwork->buf.byte.one, 1, 1, bitwork->file);
        bitwork->buf.all>>=(32-bitwork->buf_size);
	bitwork->buf_size -= 8;
}
