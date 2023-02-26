/* Used for generating a file with random amount of random symbols */

int main (int argc, char **argv) {
	FILE *in = argc>1 ? fopen(argv[1],"w") : stdin;
	if(in==NULL) {
		printf("ERROR: Cannot open %s\n", argv[1]);
		return 1;
	}
	/* Generate random symbols into 'in' file */
	fclose(in);
	return 0;
}
