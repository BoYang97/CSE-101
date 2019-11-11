#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"List.h"

#define MAX_LEN 255

int main(int argc, char* argv[]) {
    FILE *in, *out;

    char line[MAX_LEN];
    char* token; 

    int line_count = 0;
    int count = 0;
    List L = newList();


    if (argc != 3) {
        printf("Usage: %s input file output file \n", argv[0]);
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

    while (fgets(line, MAX_LEN, in) != NULL) {
        line_count++;
    }

    char* string_list[line_count];

    for (int i = 0; i < line_count; i++) {
        string_list[i] = (char*)malloc(sizeof(line));
    }

    rewind(in); 
    append(L, 0);

    while (fgets(line, MAX_LEN, in) != NULL) {
        strcpy(string_list[count], line);
        count++;
    }

    fclose(in);

    // use insertion sort to organize printed list
    for (int i = 1; i < count; i++) {
        int j = i - 1;
        moveBack(L);

        token = string_list[i];

        while (j >= 0 && strcmp(token, string_list[get(L)]) < 0) {
            movePrev(L);
            j--;
        }

        if (index(L) < 0)
            prepend(L, i);
        else
            insertAfter(L, i);
        
    }

    moveFront(L);
    while (index(L) >= 0) {
    	fprintf(out, "%s\n", string_list[get(L)]);
    	moveNext(L);
    }

    for (int i = 0; i < line_count; i++)
    	free(string_list[i]);

    fclose(out);
    freeList(&L);

    return (EXIT_SUCCESS);
}
