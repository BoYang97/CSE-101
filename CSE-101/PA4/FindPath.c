//	Julian Lehrer
//	jmlehrer
//	Programming assignment 4

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

int main (int argc, char* argv[]) {
	//variables
	FILE *in;
	FILE *out;

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

	int n; //temp for size of Graph
	fscanf(in, "%d", &n);
	Graph G = newGraph(n);

	int u, v; //temps edges
	while (fscanf(in, "%d %d", &u, &v)) {
		if (u == 0 && v == 0) //end at "dummy line"
			break;
		addEdge(G, u, v);
	}

	printGraph(out, G); 
	fprintf(out, "\n");

	//print distances
	int source, dest;

	while (fscanf(in, "%d %d", &source, &dest)) {
		List curr_path = newList();
		if (source == 0 && dest == 0)
			break;
		BFS(G, source);
		getPath(curr_path, G, dest);
		if (getDist(G, dest) == INF) { //inf distance --> no path
			fprintf(out, "The distance from %d to %d is infinity\n", source, dest);
			fprintf(out, "No %d-%d path exists\n\n", source, dest);
		} else { //print path from temp path list
			fprintf(out, "The distance from %d to %d is %d\n", source, dest, getDist(G, dest));
			fprintf(out, "A shortest %d-%d path is: ", source, dest);
			printList(out, curr_path);
			fprintf(out, "\n\n");
		}
		// clear(curr_path); //NOT WORKING
		freeList(&curr_path);
	}

    fclose(in);
    fclose(out);

    return (0);
}
