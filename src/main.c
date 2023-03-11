#include <stdio.h>
#include <stdlib.h>
#include "counter.h" /* count_symbols */
#include "heap.h" /* organize_heap */
#include "csheet.h" /* make_cmprs_list */
#include "compressor.h" /* compress, decompress */
#include <getopt.h> /* getopt */

int main (int argc, char **argv) {
	char c;
	int opt, VERBOSE = 0, L = 8;
	FILE *in = NULL;
	FILE *out = NULL;
	heap_t heap;
	csheet_t csheet;
	while ((opt = getopt(argc, argv, "hL:v:i:o:")) != -1) {
		switch (opt) {
			case 'h':  /* help */
				printf("\ncompress -i [infile] -o [outfile] [-h|-v|-p|-L]\n"
						"If infile is compressed, it'll be decompressed into out file, otherwise it'll compress it.\n"
						"\t-h:\t\tprints help.\n"
						"\t-v [value]:\t1: prints step by step process, 2: also prints csheet, 3: also prints LIST.\n"
						"\t-p [argument]:\tadd password.\n"
						"\t-L [value]:\t1: reads per byte, 2: reads per 12 bits, 3: reads per 2 bytes\n"); /* ! finish -L help */
				return 0;
			case 'v':
				VERBOSE = atoi(optarg) == 3 ? 3 : atoi(optarg) == 2 ? 2 : 1;
				/* 2 additionally prints csheet and 3 also prints LIST with added blank nodes */
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
			case 'L':
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
	c = fgetc(in);
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
		heap = count_symbols(in, L, VERBOSE); /* !set option for bit size */
		if(heap == NULL)
			return 1;
		heap = organize_heap(heap, VERBOSE);
       		csheet = make_cmprs_list(heap, VERBOSE);
		if(VERBOSE>1) {
			csheet_t tmp = csheet; int counting = 1;
			fprintf(stderr,"\nCSHEET");
			do {
                                int j = 0;
                                fprintf(stderr, "\n%d. symbol ascii: %d\t\tcode: ", counting++, tmp->symbol.numeric);
                                while(tmp->code[j] != '2')
                                        fprintf(stderr, "%c", tmp->code[j++]);
			} while( (tmp = tmp->next) != NULL);
			fprintf(stderr,"\n\n");
		}
        	compress(in, csheet, out, VERBOSE);
        	fclose(in);
        	fclose(out);
		/* !need to free csheet and heap */
	}
	return 0;
}
