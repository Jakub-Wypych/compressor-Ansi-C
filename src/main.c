/* Used to store the main function. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcpy */
#include "counter.h" /* count_symbols */
#include "heap.h" /* organize_heap */
#include "csheet.h" /* make_cmprs_list */
#include "dictionary.h" /* make_dictionary */
#include "compressor.h" /* compress */
#include "decompressor.h" /* decompress */
#include "list_iterate.h" /* read_dictionary, read_csheet, free_csheet_t */
#include "bit_funcs.h" /* showbits */
#include <getopt.h> /* getopt */

#define PATH_MAX 255

int main (int argc, char **argv) {
	char outpath[PATH_MAX], inpath[PATH_MAX], password = 0x00;
	int i;
	int opt, VERBOSE = 0, L = 8, decomp = 0;
	FILE *in = NULL, *out = NULL;
	heap_t heap;
	dictionary_t dictionary;
	csheet_t csheet;
	while ((opt = getopt(argc, argv, "hxp:L:v:i:o:")) != -1) {
		switch (opt) {
			case 'h':  /* help */
				printf("\ncompress -i [infile] -o [outfile] [-h|-v|-p|-L]\n"
						"Compresses the [infile] into [outfile], unless -x opt is on, then it'll decompress.\n"
						"\t-h:\t\tprints help.\n"
						"\t-x:\t\tdecompresses.\n"
						"\t-v [value]:\t1: prints step by step process, 2: also prints csheet, 3: also prints LIST.\n"
						"\t-p [argument]:\tadd password.\n"
						"\t-L [value]:\t1: reads per byte, 2: reads per 12 bits, 3: reads per 2 bytes\n\n");
				return 0;
			case 'x':
				decomp = 1;
				break;
			case 'p':
				i = 1;
				password = optarg[0];
				for( ; optarg[i] != '\0'; i++)
					password^=optarg[i];
				if(password == 0x00) /* just in case */
					password = 0x01;
				break;
			case 'v':
				VERBOSE = atoi(optarg) == 3 ? 3 : atoi(optarg) == 2 ? 2 : atoi(optarg) == 1 ? 1 : 0;
				/* 2 additionally prints csheet and 3 also prints LIST with added blank nodes */
				break;
			case 'i': /* (i)nput file */
				in = fopen(optarg, "rb");
				if(in==NULL) {
					fprintf(stderr,"ERROR: Failure to open %s\n", optarg);
					return 201;
				}
				strcpy(inpath, optarg);
				break;
			case 'o': /* (o)utput file */
				out = fopen(optarg, "wb");
				if(out==NULL) {
					fprintf(stderr,"ERROR: Failure to open %s\n", optarg);
					return 202;
				}
				strcpy(outpath, optarg);
				break;
			case 'L': /* (L)ength of binary reading */
				L = atoi(optarg) == 3 ? 16 : atoi(optarg) == 2 ? 12 : 8;
				break;
			case '?':
				fprintf(stderr,"ERROR with option: %c\n", optopt);
				return 205;
		}
	}
	if(in==NULL) {
		fprintf(stderr, "ERROR: No input file given. Please refer to help (-h)\n");
		if(out!=NULL)
			fclose(out);
		return 203;
	} else if(out == NULL) {
		fprintf(stderr, "ERROR: No output file given. Please refer to help (-h)\n");
                if(in!=NULL)
			fclose(in);
		return 204;
	} else if( strcmp(inpath, outpath) == 0 ) {
		fprintf(stderr, "ERROR: Input and output files are the same.\n");
		fclose(in);
		fclose(out);
		return 211;
	}
	if(decomp) {
		heap_t original_heap;
		if(VERBOSE)
			fprintf(stderr,"MAIN.C: Beginning the decompression process...\n");
	        original_heap = decompress(in, password, out, VERBOSE);
		L = original_heap->amount; /* reding symbol length from first node */
		if(L>200) { /* There were issues with decompresion */
			free(original_heap);
			fclose(in);
			fclose(out);
			return L;
		}
		heap = original_heap; /* removing the first node */
		original_heap = original_heap->next;
		free(heap);
		fclose(out);
		out = fopen(outpath, "rb");
		heap = count_symbols(out, L, 0);
		while(heap != NULL && original_heap != NULL) {
			heap_t heap_tmp = heap;
			if(heap->symbol.numeric != original_heap->symbol.numeric || heap->amount != original_heap->amount)
				break;
			heap = heap->next;
			free(heap_tmp);
			heap_tmp = original_heap;
			original_heap = original_heap->next;
			free(heap_tmp);
		}
		if(heap != NULL || original_heap != NULL) {
			fprintf(stderr, "ERROR: The compressed file was damaged!\n");
			fclose(in);
			fclose(out);
			return 210;
		}
		if(heap != NULL)
			free_heap_t(heap);
		if(original_heap != NULL)
			free_heap_t(original_heap);	
	} else {
		if(VERBOSE)
			fprintf(stderr, "MAIN.C: Beginning the compression process...\n");
		heap = count_symbols(in, L, VERBOSE);
		if(heap == NULL) {
			fclose(in);
			fclose(out);
			return 206;
		}
		dictionary = make_dictionary(heap);
		heap = organize_heap(heap, VERBOSE);
       		csheet = make_cmprs_list(heap, VERBOSE);
		if(VERBOSE>1) {
			read_dictionary(dictionary);
			read_csheet(csheet);
		}
        	compress(dictionary, L, csheet, in, out, password, VERBOSE);
		free_csheet_t(csheet);
	}
	fclose(in);
	fclose(out);
	return 0;
}
