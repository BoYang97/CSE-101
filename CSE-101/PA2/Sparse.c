#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "Matrix.h"

#define MAX_LEN 255

int main(int argc, char* argv[]) {
    FILE *in, *out;

    char l[MAX_LEN];
    if (argc != 3) {
        printf("Usage: %s <input file> <output file> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Unable to read to file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    out = fopen(argv[2], "w");
    if (out == NULL){
        printf("Unable to write to file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    int size;
    int e_1;
    int e_2; 

    if (fgets(l, sizeof(l), in) != NULL && l[0] != '\n') {
        sscanf(l, "%d %d %d", &size, &e_1, &e_2);
    }

    Matrix A = newMatrix(size);
    Matrix B = newMatrix(size);

    printf("%s\n", l);
    fclose(in);
    fclose(out);
    return (EXIT_SUCCESS);
}
