/* Used to compare file sizes */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	double f1, f2;
	FILE *uncompressed = fopen(argv[1], "rb");
	FILE *compressed = fopen(argv[2], "rb");
	fseek(uncompressed, 0, SEEK_END);
	fseek(compressed, 0, SEEK_END);
	f1 = ftell(uncompressed);
	f2 = ftell(compressed);
	fclose(uncompressed);
	fclose(compressed);
	printf("Compression rate of %g percent\n", (f2/f1)*100);
	return 0;
}
