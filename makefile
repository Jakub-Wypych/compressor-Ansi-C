all:
	cc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	cc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncompressed
	rm data/gen

compress_no_opt: all
	bin/compressor -i data/uncompressed -o data/compressed

compress_verbose: all
	bin/compressor -i data/uncompressed -o data/compressed -v

decompress_no_opt: all
	bin/compressor -i data/compressed -o data/decompressed -d

test_compress_no_symbol: all
	bin/compressor data/no_symbols data/decompressed

test_compress_one_symbol: all
	bin/compressor data/only_one_symbol data/decompressed
