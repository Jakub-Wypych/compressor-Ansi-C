/* Used to store the main function. */

#include <stdio.h>
#include <stdlib.h>
#include "counter.h" /* count_symbols */
#include "heap.h" /* organize_heap */
#include "csheet.h" /* make_cmprs_list */
#include "dictionary.h" /* make_dictionary */
#include "compressor.h" /* compress */
#include "decompressor.h" /* decompress */
#include "list_iterate.h" /* read_dictionary, read_csheet, free_csheet_t */
#include "bit_funcs.h" /* showbits */
#include <getopt.h> /* getopt */

int main (int argc, char **argv) {
	char c;
	int opt, VERBOSE = 0, L = 8;
	FILE *in = NULL;
	FILE *out = NULL;
	heap_t heap;
	dictionary_t dictionary;
	csheet_t csheet;
	while ((opt = getopt(argc, argv, "hL:v:i:o:")) != -1) {
		switch (opt) {
			case 'h':  /* help */
				printf("\ncompress -i [infile] -o [outfile] [-h|-v|-p|-L]\n"
						"Compresses the [infile] into [outfile], unless -x opt is on, then it'll decompress.\n"
						"\t-h:\t\tprints help.\n"
						"\t-x:\t\tdecompresses.\n"
						"\t-v [value]:\t1: prints step by step process, 2: also prints csheet, 3: also prints LIST.\n"
						"\t-p [argument]:\tadd password.\n"
						"\t-L [value]:\t1: reads per byte, 2: reads per 12 bits, 3: reads per 2 bytes\n");
				return 0;
			case 'v':
				VERBOSE = atoi(optarg) == 3 ? 3 : atoi(optarg) == 2 ? 2 : 1;
				/* 2 additionally prints csheet and 3 also prints LIST with added blank nodes */
				break;
			case 'i': /* (i)nput file */
				in = fopen(optarg, "rb");
				if(in==NULL) {
					fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
					return 1;
				}
				break;
			case 'o': /* (o)utput file */
				out = fopen(optarg, "wb");
				if(out==NULL) {
					fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
					return 1;
				}
				break;
			case 'L': /* (L)ength of binary reading */
				L = atoi(optarg) == 3 ? 16 : atoi(optarg) == 2 ? 12 : 8;
				break;
			case '?':
				fprintf(stderr,"Unknown option: %c\n", optopt);
				return 1;
		}
	}
	if(in==NULL || out==NULL) {
		fprintf(stderr, "ERROR: No input/output file given. Please refer to help (-h)\n");
		return 1;
	}
	c = fgetc(in); /* !change later to use mask and -x opt */
	if((c == 'E' || c == 'U') && (c = fgetc(in)) == 'C') {
		if(VERBOSE)
			fprintf(stderr,"MAIN.C: Beginning the decompression process...\n");
	        decompress(in, out, VERBOSE);
	        fclose(in);
		fclose(out);
	}
	else {
		if(VERBOSE)
			fprintf(stderr, "MAIN.C: Beginning the compression process...\n");
		heap = count_symbols(in, L, VERBOSE);
		if(heap == NULL) {
			fclose(in);
			fclose(out);
			return 1;
		}
		dictionary = make_dictionary(heap);
		heap = organize_heap(heap, VERBOSE);
       		csheet = make_cmprs_list(heap, VERBOSE);
		if(VERBOSE>1) {
			read_dictionary(dictionary);
			read_csheet(csheet);
		}
        	compress(dictionary, L, csheet, in, out, 0xFF, VERBOSE);
		/* !remove */
		rewind(in);
		while( fread(&c, 1, 1, in) != 0)
			showbits(c, 8);
		printf("\n");
		/* remove! */
		fclose(in);
        	fclose(out);
		/* !remove */
		in = fopen("data/compressed", "rb");
		while( fread(&c, 1, 1, in) != 0)
                        showbits(c, 8);
		printf("\n");
		fclose(in);
		/* remove! */
		free_csheet_t(csheet);
	}
	return 0;
}
