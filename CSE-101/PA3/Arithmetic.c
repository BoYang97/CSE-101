#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BigInteger.h"

int MAX = 100000000;

int main(int argc, char* argv[]) {
	FILE* in;
	FILE* out;
	// char line[MAX];
	// char len_big_int_1[MAX];
	// char len_big_int_2[MAX];

	if (argc != 3) {
		printf("Usage: %s <input> <output>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	in = fopen(argv[1], "r");

	if (in == NULL) {
		printf("ERROR: unable to read input file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	out = fopen(argv[2], "w");

	if (out == NULL) {
		printf("ERROR: unable to read output file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	int line_to_get = 0;
	while (fgets(line, sizeof(line), file)) {
		line_to_get++;
		if (line_to_get == 1) 
			len_big_int_1 = line;
		if (line_to_get == 3)
			len_big_int_2 = line;
	}

	// int s1 = *((int*) len_big_int_1);
	// int s2 = *((int*) len_big_int_2);

	// char big_int_1[s1];
	// char big_int_2[s2];

	// rewind(in);

	// BigInteger A = stringToBigInteger(big_int_1);
	// BigInteger B = stringToBigInteger(big_int_2);
	BigInteger A = newBigInteger();

	return (0);
}