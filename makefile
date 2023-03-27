all:
	cc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	cc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncompressed
	rm data/gen

file_compare:
	cc -Wall -ansi -pedantic test/cmp_files.c -o test/compare
	test/compare data/uncompressed data/decompressed
	rm test/compare

compress_no_opt: all
	bin/compressor -i data/uncompressed -o data/compressed

decompress_no_opt: all
	bin/compressor -i data/compressed -o data/decompressed -x

decompress: all
	bin/compressor -i data/compressed -o data/decompressed -x -v 3 -L 1

help: all
	bin/compressor -h

test_8_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 1

test_12_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 2

test_16_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 3

test_memory_leak_compress: all
	valgrind bin/compressor -i data/uncompressed -o data/compressed -L 1

test_memory_leak_decompress: all
	valgrind bin/compressor -i data/compressed -o data/decompressed -L 1 -x
