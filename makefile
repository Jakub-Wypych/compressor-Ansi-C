all:
	cc -Wall -ansi -pedantic src/*.c -o bin/compressor

gen_uncompressed_file:
	cc -Wall -ansi -pedantic data/gen_uncmprs_file.c -o data/gen
	data/gen data/uncompressed
	rm data/gen

file_compare:
	cmp data/uncompressed data/decompressed
	@echo Files are the same :D

size_compare:
	cc -Wall -ansi -pedantic test/cmp_size.c -o test/compare
	test/compare data/uncompressed data/compressed
	rm test/compare

compress_no_opt: all
	bin/compressor -i $(in) -o $(out)

decompress_no_opt: all
	bin/compressor -i $(in) -o $(out) -x

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
	cmp data/lorem_ipsum data/decompressed
	@echo Files are the same :D
	cc -Wall -ansi -pedantic test/cmp_size.c -o test/compare
	test/compare data/lorem_ipsum data/compressed
	rm test/compare

test_error_201: all
	valgrind bin/compressor -i no_file -o data/compressed -v 1

test_error_203: all
	valgrind bin/compressor -o data/compressed -v 1

test_error_204: all
	valgrind bin/compressor -i data/uncompressed -v 1

test_error_205: all
	valgrind bin/compressor -q

test_error_206: all
	valgrind bin/compressor -i test/unreadable_per_12 -o data/compressed -v 1 -L 2

test_error_207: all
	bin/compressor -i data/uncompressed -o data/compressed -p xxx
	valgrind bin/compressor -i data/compressed -o data/decompressed -v 1 -x

test_error_208: all
	bin/compressor -i data/uncompressed -o data/compressed -p xxx
	valgrind bin/compressor -i data/compressed -o data/decompressed -p yyy -x -v 1

test_error_211: all
	bin/compressor -i data/uncompressed -o data/uncompressed
