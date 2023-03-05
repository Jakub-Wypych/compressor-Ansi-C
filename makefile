all:
	cc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	cc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncompressed
	rm data/gen

compress_no_opt: all
	bin/compressor -i data/uncompressed -o data/compressed

verbose: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3

help: all
	bin/compressor -h

test_compress_no_symbol: all
	bin/compressor -i data/no_symbols -o data/decompressed -v 3

test_compress_one_symbol: all
	bin/compressor -i data/only_one_symbol -o data/decompressed -v 3
