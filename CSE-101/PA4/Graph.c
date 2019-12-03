//	Julian Lehrer
//	jmlehrer
//	Programming assignment 4

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "Graph.h"

typedef struct GraphObj {
	List *adjacent;
	int *color;
	int *parent;
	int *distance;
	int order; //vertices
	int size; //edges
	int source;
} GraphObj;

Graph newGraph(int n) {
	Graph G = malloc(sizeof(GraphObj));
	assert(G != NULL);
	G->order = n;
	G->size = 0;
	G->source = NIL;
	
	// G->parent = malloc(sizeof(int) * (n + 1));
	// G->color = malloc(sizeof(int) * (n + 1));
	// G->distance = malloc(sizeof(int) * (n + 1));
	// G->adjacent = malloc(sizeof(List) * (n + 1));
    G->adjacent = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));

	for (int i = 1; i <= n; i++) {
		G->adjacent[i] = newList();
		G->parent[i] = NIL;
		G->color[i] = WHITE;
		G->distance[i] = INF;
	}			
	return (G);
}

void freeGraph(Graph* pG) {
	if (pG != NULL && *pG != NULL) {
		for (int i = 1; i <= getOrder(*pG); i++) {
			freeList(&(*pG)->adjacent[i]);
		}
		free((*pG)->adjacent);
		free((*pG)->parent);
		free((*pG)->distance);
		free((*pG)->color);
		free((*pG));
		*pG = NULL;
	}
}

/*** Access functions ***/
int getOrder(Graph G) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getOrder on a null Graph object\n");
		exit(1);
	}
	return (G->order);
}

int getSize(Graph G) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getSize on a null Graph object\n");
		exit(1);
	}
	return (G->size);
}

int getSource(Graph G) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getSource on a null Graph object\n");
		exit(1);
	}
	return (G->source);
}

int getParent(Graph G, int u) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getParent on a null Graph object\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		fprintf(stderr, "Error in module Graph: cannot call getParent on an invalid vertex index\n");
		exit(1);
	}
	return (G->parent[u]);
}

int getDist(Graph G, int u) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getDist on a null Graph object\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) { //preconditions
		fprintf(stderr, "Error in module Graph: cannot call getDist on an invalid vertex index\n");
		exit(1);
	}
	if (getSource(G) == NIL)
		return INF;
	return (G->distance[u]);
}

void getPath(List L, Graph G, int u) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call getPath on a null Graph object\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G)) {
		fprintf(stderr, "Error in module Graph: cannot call getPath on an invalid vertex index\n");
		exit(1);
	}
	if (getSource(G) == NIL) {
		fprintf(stderr, "Error in module Graph: cannot call getPath on an undefined source vertex\n");
		exit(1);
	}

	if (u == G->source) { 
		append(L, G->source);
	} else if (G->parent[u] == NIL) { //base of recursion
		append(L, NIL);
	} else { //recur through the path after BFS and record it
		getPath(L, G, G->parent[u]);
		append(L, u);
	}
}

/*** Manipulation procedures ***/
void makeNull(Graph G) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call makeNull on a null Graph object\n");
		exit(1);
	}
	for (int i = 1; i <= getOrder(G); i++) { //resetting other conditions may be uncessary...?
		clear(G->adjacent[i]);
		// G->color[i] = WHITE;
		// G->distance[i] = INF;
		// G->parent = NIL;
	}
	G->size = 0;
	// G->source = NIL;
}

void addEdge(Graph G, int u, int v) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call addEdge on a null Graph object\n");
		exit(1);
	}
	if (u < 1 || v < 1 || u > getOrder(G) || v > getOrder(G)) {
		fprintf(stderr, "Error in module Graph: cannot call addEdge on invalid vertices\n");
	}

	addArc(G, u, v);
	addArc(G, v, u);
	G->size--; //remove 1 for double counting arc creations
}

void addArc(Graph G, int u, int v) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call addArc on a null Graph object\n");
		exit(1);
	}
	if (u < 1 || v < 1 || u > getOrder(G) || v > getOrder(G)) {
		fprintf(stderr, "Error in module Graph: cannot call addArc on invalid vertices\n");
	}

    if (length(G->adjacent[u]) == 0) {
        append(G->adjacent[u], v);
    } else { 
    	//walk down adjaceny list and insert in order
	    moveFront(G->adjacent[u]);
	    while (index(G->adjacent[u]) != -1) {

	    	if (v > get(G->adjacent[u])) {
	    		moveNext(G->adjacent[u]);
	    	} else {
	    		insertBefore(G->adjacent[u], v);
	    		G->size++;
	    		return;
	    	}
	    }
        append(G->adjacent[u], v);
	}
	G->size++;
}

void BFS(Graph G, int s) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call BFS on a null Graph object\n");
		exit(1);
	}
	//IMPLEMENTATION OF BFS PSEUDOCODE
	//	1. for 𝑥 ∈ V(G) − {s}
	//	2. color[𝑥] = white
	//	3. 𝑑[𝑥] = ∞
	//	4. 𝑝[𝑥] = nil
	for (int i = 1; i <= getOrder(G); i++) {
		G->parent[i] = NIL;
		G->color[i] = WHITE;
		G->distance[i] = INF;
	}

	//	5. color[𝑠] = gray
	//	6. 𝑑[𝑠] = 0
	//	7. 𝑝[𝑠] = nil
	G->source = s;

	G->color[s] = GREY;
	G->distance[s] = 0;
	G->parent[s] = NIL;

	//	8. 𝑄 = ∅ // construct a new empty queue
	//	9. Enqueue(𝑄, 𝑠)
	List Q = newList();
	append(Q, s);

	//10. while 𝑄 ≠ ∅
	//	11. 𝑥 = Dequeue(𝑄)
	//	12. for 𝑦 ∈ adj[𝑥]
	//	13. if color[𝑦] == white // 𝑦 is undiscovered
	//	14. color[𝑦] = gray // discover 𝑦
	//	15. 𝑑[𝑦] = 𝑑[𝑥] + 1
	//	16. 𝑝[𝑦] = 𝑥
	//	17. Enqueue(𝑄, 𝑦)
	//	18. color[𝑥] = black // finish x
	//END OF BFS PSEUDOCODE

	while (length(Q) > 0) {	
		int x = front(Q);
		deleteFront(Q);
		moveFront(G->adjacent[x]);

		while (index(G->adjacent[x]) != -1) {
			int y = get(G->adjacent[x]);
			if (G->color[y] == WHITE) {
				G->color[y] = GREY;
				G->distance[y] = G->distance[x] + 1;
				G->parent[y] = x;
				append(Q, y);
			}
			moveNext(G->adjacent[x]);
		}
		G->color[x] = BLACK;
	}
	freeList(&Q);
}

/*** Other operations ***/
void printGraph(FILE* out, Graph G) {
	if (G == NULL) {
		fprintf(stderr, "Error in module Graph: cannot call printGraph on a null Graph object\n");
		exit(1);
	}
	for (int i = 1; i <= getOrder(G); i++) {
		fprintf(out, "%d: ", i);
		printList(out, G->adjacent[i]);
		fprintf(out, "\n");
	}
}

