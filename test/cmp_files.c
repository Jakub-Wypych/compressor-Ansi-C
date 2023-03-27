/* Used for checking if files are the same */

#include <stdio.h>

int main( int argc, char **argv) {
	FILE *uncompressed = fopen(argv[1], "rb");
	FILE *decompressed = fopen(argv[2], "rb");
	char c1, c2;
	int l1 = 1, l2 = 1;
	while( l1 != 0 && l2 != 0 ) {
		l1 = fread(&c1, 1, 1, uncompressed);
	        l2 = fread(&c2, 1, 1, decompressed);
		if(c1 != c2) {
			printf("ERROR: Diffrent symbol found!\n");
			fclose(uncompressed);
	        	fclose(decompressed);
        		return 1;
		}
	}
	if(l1 != l2) {
		printf("ERROR: Diffrent lenghts!\n");
		fclose(uncompressed);
	        fclose(decompressed);
	        return 1;
	} else
		printf("Files are the same. :D\n");
	fclose(uncompressed);
	fclose(decompressed);
	return 0;
}
