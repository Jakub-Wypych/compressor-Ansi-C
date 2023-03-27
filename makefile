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

size_compare:
	cc -Wall -ansi -pedantic test/cmp_size.c -o test/compare
	test/compare data/uncompressed data/compressed
	rm test/compare

compress_no_opt: all
	bin/compressor -i data/uncompressed -o data/compressed

decompress_no_opt: all
	bin/compressor -i data/compressed -o data/decompressed -x

help: all
	bin/compressor -h

test_8_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 1

test_12_bit: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 2

test_2_byte: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 3

test_decompress: all
	bin/compressor -i data/compressed -o data/decompressed -x -v 3

test_password: all
	bin/compressor -i data/uncompressed -o data/compressed -v 3 -L 1 -p polibuda
	bin/compressor -i data/compressed -o data/decompressed -x -v 3 -p polibuda

test_memory_leak_compress: all
	valgrind bin/compressor -i data/uncompressed -o data/compressed -L 1

test_memory_leak_decompress: all
	valgrind bin/compressor -i data/compressed -o data/decompressed -L 1 -x

lorem_ipsum: all
	bin/compressor -i data/lorem_ipsum -o data/compressed -v 1 -L 1
	bin/compressor -i data/compressed -o data/decompressed -v 1 -x
	cc -Wall -ansi -pedantic test/cmp_files.c -o test/compare
	test/compare data/lorem_ipsum data/decompressed
	rm test/compare
	cc -Wall -ansi -pedantic test/cmp_size.c -o test/compare
	test/compare data/lorem_ipsum data/compressed
	rm test/compare

