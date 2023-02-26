#include <stdio.h>
#include <stdlib.h>
#include "counter.h" /* count_symbols */
#include "heap.h" /* organize_heap */
#include "csheet.h" /* make_cmprs_list */
#include "compressor.h" /* compress, decompress */
#include <getopt.h> /* getopt */

int main (int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "hvi:o:")) != -1) {
		switch (opt) {
			case 'h': {  /* help */
					  printf("\n"); /* !finish writing help */
					  return 0;
				  }
			case 'v': {
					  #define VERBOSE
					  break;
				  }
			case 'i': { /* (i)nput file */
					  FILE *in = fopen(optarg, "r"); /* !check if readable and act accordingly */
					  if(in==NULL) {
						  fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
						  return 1;
					  }
					  break;
				  }
			case 'o': { /* (o)utput file */
					  FILE *out = fopen(optarg, "w"); /* !check if writable and act accordingly */
					  if(out==NULL) {
						  fprintf(stderr,"ERROR: Failure to read %s\n", optarg);
						  return 1;
					  }
					  break;
				  }
			case '?': {
					  printf("Unknown option: %c\n", optopt);
					  return 1;
				  }
		}
	}
	/* !depending on option compress or decompress */
	/*decompress(in, out);
	fclose(in);
	fclose(out);
	return 0;
	
	heap_t heap = count_symbols(in);
	organize_heap(heap);
	csheet_t csheet = make_cmprs_list(heap);
	compress(in, csheet, out);
	fclose(in);
	fclose(out);
	return 0;*/
	return 0;
}
