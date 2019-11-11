#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "BigInteger.h"

int MAX = 1000000;

int main(int argc, char* argv[]) {
	BigInteger A = newBigInteger();
	BigInteger B = newBigInteger();
	FILE* in;
	BigInteger C = newBigInteger();
	BigInteger D = newBigInteger();
	BigInteger E = newBigInteger();
	BigInteger F = newBigInteger();

	in = fopen(argv[1], "r");

	int n_lines = 0;
	char input[MAX];
	char *lines[4];

	while (fgets(input, sizeof(input), in)) {
		lines[n_lines] = strdup(input);
		n_lines++;
	}

	for (int i = 1 ; i < 4; i++) {
		printf("lines[%d] is: %s\n", i, lines[i]);
	}

	A = stringToBigInteger(lines[1]);
	B = stringToBigInteger(lines[3]);

	// add(C, A, B);
	subtract(D, A, B);
	printf("\nPrinting BigInteger A \n");
	printBigInteger(stdout, A);
	printf("\nPrinting BigInteger B \n");
	printBigInteger(stdout, B);
	// subtract(D, A, B);
	printf("\n\n\n");
	printf("sign of A: %d\n", sign(A));
	printf("sign of B: %d\n", sign(B));
	// printf("\nPrinting BigInteger C \n");
	// printBigInteger(stdout, C);
	printf("\nPrinting BigInteger D \n");
	printBigInteger(stdout, D);
	// printf("\nPrinting BigInteger E \n");
	// printBigInteger(stdout, E);

	return (0);
}