#include <stdio.h>
#include <counter.h> /* count_symbols */
#include <heap.h> /* organize_heap */
#include <csheet.h> /* make_cmprs_list */
#include <compressor.h> /* compress, decompress */
#include <unistd.h> /* getopt */

int main (int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "hvi:o:")) != -1) {
		switch (opt) {
		case 'h': /* help */
			printf("\n"); /* !finish writing help */
			return 0;
		case 'v':
			#define VERBOSE
			break;
		case 'i': /* (i)nput file */
			FILE *in = fopen(argv[1], "r"); /* !check if readable and act accordingly */
			break;
		case 'o': /* (o)utput file */
			FILE *out = fopen(argv[2], "w"); /* !check if writable and act accordingly */
			break;
		case '?':
			printf(“Unknown option: %c\n”, optopt);
                	exit(EXIT_FAILURE);
		}
	}
	/* !depending on option compress or decompress */
	decompress(in, out);
	fclose(in);
	fclose(out);
	return 0;
	
	heap_t heap = count_symbols(in);
	organize_heap(heap);
	csheet_t csheet = make_cmprs_list(heap);
	compress(in, csheet, out);
	fclose(in);
	fclose(out);
	return 0;
}
