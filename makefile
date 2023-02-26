all:
	gcc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	gcc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncrompressed
	rm data/gen

compress_no_opt: all
	bin/compressor data/uncrompressed data/compressed

decompress_no_opt: all
	bin/compressor data/compressed data/decompressed

test_compress_no_symbol: all
	bin/compressor data/no_symbols data/decompressed

test_compress_one_symbol: all
	bin/compressor data/only_one_symbol data/decompressed
