all:
	cc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	cc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncompressed
	rm data/gen

compress_no_opt: all
	bin/compressor -i data/uncompressed -o data/compressed

verbose: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 1

help: all
	bin/compressor -h

test_compress_no_symbol: all
	bin/compressor -i data/no_symbols -o data/decompressed -v 3

test_compress_one_symbol: all
	bin/compressor -i data/only_one_symbol -o data/decompressed -v 3

test_12_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 2

test_16_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 3

test_memory_leak: all
	valgrind bin/compressor -i data/uncompressed -o data/compressed -L 1
