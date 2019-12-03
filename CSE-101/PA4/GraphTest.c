//	Julian Lehrer
//	jmlehrer
//	Programming assignment 4

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
	Graph A = newGraph(100);
	Graph B = newGraph(100);

	for (int i = 1; i <= 100; i++) {
		addEdge(A, i, i + 1);
		addEdge(B, i, i % 4);
	}

	printGraph(stdout, A);
	printf("\n");
	printGraph(stdout, B);


	printf("FOR A: ORDER IS: %d SIZE IS: \n", getOrder(A), getSize(A));

	makeNull(A);
	printf("FOR A: ORDER IS: %d SIZE IS: \n", getOrder(A), getSize(A));

	return (0);
}