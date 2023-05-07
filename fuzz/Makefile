zsv-fuzz: zsv-fuzz.c
	clang -g -fsanitize=fuzzer,memory,undefined,nullability -I../include -o zsv-fuzz zsv-fuzz.c ../src/zsv.c