/* Used for generating a file with random amount of random symbols */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char **argv) {
	FILE *out = argc>1 ? fopen(argv[1],"w") : stdin;
	int how_many_symbols;
	int how_many_times;
	int symbol_number;
	int i, j;
	if(out==NULL) {
		printf("ERROR: Cannot open %s\n", argv[1]);
		return 1;
	}
	srand(time(NULL));
	how_many_symbols = (int) rand()%51; /* we can have 0 to 50 diffrent symbols */
	for(i=0;i<=how_many_symbols;i++) {
		symbol_number = (int) rand()%256; /* we can have a symbol number from 0 to 255 */
		how_many_times = (int) rand()%100+1; /* it can appear from 1 to 100 times */
		for(j=0;j<=how_many_times;j++)
			fprintf(out,"%c",symbol_number);
	}
	fclose(out);
	return 0;
}
