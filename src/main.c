#include <stdio.h>
#include <stdlib.h>
#include "counter.h" /* count_symbols */
#include "heap.h" /* organize_heap */
#include "csheet.h" /* make_cmprs_list */
#include "compressor.h" /* compress, decompress */
#include <getopt.h> /* getopt */

int main (int argc, char **argv) {
	int opt;
	int decomp = 0;
	int VERBOSE = 0;
	FILE *in = NULL;
	FILE *out = NULL;
	heap_t heap;
	csheet_t csheet;
	while ((opt = getopt(argc, argv, "dhvi:o:")) != -1) {
		switch (opt) {
			case 'h':  /* help */
				printf("\n"); /* !finish writing help */
				return 0;
				break;
			case 'v':
				VERBOSE = 1;
				break;
			case 'd':
				decomp = 1; /* we'll be decompressing the infile */
				break;
			case 'i': /* (i)nput file */
				in = fopen(optarg, "r");
				if(in==NULL) {
					fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
					return 1;
					}
				break;
			case 'o': /* (o)utput file */
				out = fopen(optarg, "w");
				if(out==NULL) {
					fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
					return 1;
				}
				break;
			case '?':
				fprintf(stderr,"Unknown option: %c\n", optopt);
				return 1;
				break;
		}
	}
	if(in==NULL || out==NULL) {
		fprintf(stderr, "ERROR: No input/output file given. Please refer to help (-h)\n");
		return 1;
	}
	if(decomp) {
	        decompress(in, out, VERBOSE);
	        fclose(in);
		fclose(out);
	}
	else {
		if(VERBOSE)
			fprintf(stderr, "MAIN.C: Beginning the compression process...\n");
		heap = count_symbols(in, VERBOSE);
		organize_heap(heap, VERBOSE);
       		csheet = make_cmprs_list(heap, VERBOSE);
        	compress(in, csheet, out, VERBOSE);
        	fclose(in);
        	fclose(out);
		/* !need to free csheet and heap */
	}
	return 0;
}
