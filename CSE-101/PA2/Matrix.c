#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Matrix.h"
#include "List.h"

#define MAX_LEN 1000000

typedef struct EntryObj {
	int col;
	double value;
} EntryObj;

typedef EntryObj* Entry;

Entry newEntry(int column, double val) {
	Entry E = malloc(sizeof(EntryObj));
	E->col = column;
	E->value = val;
	return E;
}

void deleteEntry(Entry *ent) { 
	if (ent != NULL && *ent != NULL) {
		free(ent);
	}
	*ent = NULL;
}

typedef struct MatrixObj {
	int nxn;
	int nonzero;
	List arr[MAX_LEN];
} MatrixObj;

int dotProduct(List firstRow, List secondRow) { //hope this works for now...
	double sum = 0;
	moveFront(firstRow);
	moveFront(secondRow);
	while (index(firstRow) != -1 && index(secondRow) != -1) {
		if (((Entry)get(firstRow))->col < ((Entry)get(secondRow))->col) {
			moveNext(firstRow);
		}
		else if (((Entry)get(firstRow))->col > ((Entry)get(secondRow))->col) {
			moveNext(secondRow);
		}
		else {
			sum += (((Entry)get(firstRow))->value) * (((Entry)get(secondRow))->value);
			moveNext(firstRow);
			moveNext(secondRow);
		}
	}
	return sum;
}

Matrix newMatrix(int n) {
	Matrix M = malloc(sizeof(MatrixObj));
	assert (M != NULL);
	M->nxn = n;
	for (int i = 1; i <= n; i++) {
		M->arr[i] = newList();
	}
	return M;
}

void freeMatrix(Matrix* pM) {
	if (pM != NULL && *pM != NULL) {
		for (int i = 0; i < (*pM)->nxn; i++) {
			freeList(&((*pM)->arr[i]));
		}
		free(*pM);
		*pM = NULL;
	}
}

// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M) {
	return M->nxn;
}

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M) {
	int num = 0;
	for (int i = 1; i <= M->nxn; i++) {
		num += length(M->arr[i]); //M->arr[i] should be a list object, which has a length property
	}
	return num;
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) { //if they arent the same size then this is easy
	if (size(A) != size(B))
		return 0;
	for (int i = 1; i <= size(A); i++) { // walk down lists together, why isnt this working....
		List currentListA = A->arr[i]; 
		List currentListB = B->arr[i];
		if (length(currentListA) > 0 && length(currentListA) > 0) {
			moveFront(currentListA);
			moveFront(currentListB);
			if (length(currentListA) != length(currentListB)) {
				return 0;
			} else {
				while(index(currentListA) != -1) {
					if (((Entry)get(currentListA))->col != ((Entry)get(currentListB))->col || ((Entry)get(currentListA))->value != ((Entry)get(currentListB))->value) {
						return 0;
					}
					moveNext(currentListA);
					moveNext(currentListB);
				}
			}
		} else {
			return 0;
		}
	}
	return 1;
}

// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix.
void makeZero(Matrix M) {
	for (int i = 1; i <= size(M); i++) {
		clear(M->arr[i]);
	}
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
	if (i < 1 || i > size(M) || j < 1 || j > size(M)) {
		printf("Error in Matrix module: cannot call changeEntry on an invalid matrix index\n");
		exit(1);
	}
	Entry n = newEntry(j, x);
	if (length(M->arr[i]) == 0) {
		if (x != 0 || x != 0.0)
			prepend(M->arr[i], n);
	} else {
		moveFront(M->arr[i]);
		if (x != 0 || x != 0.0) {
			if (j < ((Entry)get(M->arr[i]))->col) { //less then column at the first index
				prepend(M->arr[i], newEntry(j, x));
			} else {
				moveFront(M->arr[i]);
				while (index(M->arr[i]) != -1) {
					if (j == ((Entry)get(M->arr[i]))->col) {
						((Entry)get(M->arr[i]))->value = x;
						return;
					}
					moveNext(M->arr[i]);
				}
				append(M->arr[i], n);
			}
		} else {
			moveFront(M->arr[i]);
			while (index(M->arr[i]) != -1) {
				if (j == ((Entry)get(M->arr[i]))->col)
					delete(M->arr[i]);
				moveNext(M->arr[i]);
			}
		}
	}
}

// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
    //return transpose(transpose(A)); lol
	 Matrix B = newMatrix(size(A));
	 for (int i = 1; i <= size(A); i++) {
	 	moveFront(A->arr[i]);
	 	while (index(A->arr[i]) != -1) {
	 		changeEntry(B, i, ((Entry)get(A->arr[i]))->col, ((Entry)get(A->arr[i]))->value);
	 		moveNext(A->arr[i]);
	 	}
	 }
	 return B;
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A) {
	Matrix B = newMatrix(size(A));
	for (int i = 1; i <= size(A); i++) {
		moveFront(A->arr[i]);
		while (index(A->arr[i]) > -1) {
			changeEntry(B, ((Entry)get(A->arr[i]))->col, i, ((Entry)get(A->arr[i]))->value);
			moveNext(A->arr[i]);
		}
	}
	return B;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
	Matrix B = newMatrix(size(A));
	for (int i = 1; i <= size(A); i++) {
		if (length(A->arr[i]) != 0) {
			moveFront(A->arr[i]);
			while (index(A->arr[i]) != -1) {
				Entry e = (Entry)get(A->arr[i]);
				changeEntry(B, i, e->col, x * e->value);
				moveNext(A->arr[i]);
			}
		}
	}
	return B;
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
	if (size(A) != size(B)) {
		printf("Error in Matrix module: cannot call sum() on two matrices of different sizes\n");
		exit(1);
	}
	Matrix N = newMatrix(size(A));
	List checkList;
	if (equals(A, B))
		return scalarMult(2, A);
	for (int i = 1; i <= size(A); i++) {
		List List1 = A->arr[i];
		List List2 = B->arr[i];
		moveFront(List1);
		moveFront(List2);
		while (index(List1) >= 0 && index(List2) >= 0) {
			Entry e1 = (Entry)get(List1);
			Entry e2 = (Entry)get(List2);

			if (e1->col == e2->col) {
				if (e1->value + e2->value != 0) {
					changeEntry(N, i, e1->col, (e1->value + e2->value));
					moveNext(List1);
					moveNext(List2);
				} else {
					moveNext(List1);
					moveNext(List2);
				}
			} else if (e1->col < e2->col) {
				changeEntry(N, i, e1->col, e1->value);
				moveNext(List1);
			} else {
				changeEntry(N, i, e2->col, e2->value);
			}
		}
		if (index(List1) == index(List2)) {
			if (index(List1) > -1) {
				checkList = List1;
			} else {
				checkList = List2;
			}
			while(index(checkList) > -1) {
				Entry e3 = (Entry)get(checkList);
				changeEntry(N, i, e3->col, e3->value);
				moveNext(checkList);
			}
		}
	}
	return N;
}


// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {

	if (size(A) != size(B)) {
		printf("Error in Matrix module: cannot call diff() on two matrices of different sizes\n");
		exit(1);
	}
	List checkList;
	int s = -1;
	Matrix N = newMatrix(size(A));
	for (int i = 1; i <= size(A); i++) {
		List List1 = A->arr[i];
		List List2 = B->arr[i];
		moveFront(List1);
		moveFront(List2);

		while (index(List1) >= 0 && index(List2) >= 0) {
			Entry e1 = (Entry)get(List1);
			Entry e2 = (Entry)get(List2);

			if (e1->col == e2->col) {
				if (e1->value - e2->value != 0) {
					changeEntry(N, i, e1->col, (e1->value - e2->value));
					moveNext(List1);
					moveNext(List2);
				} else {
					moveNext(List1);
					moveNext(List2);
				}
			} else if (e1->col < e2->col) {
				changeEntry(N, i, e1->col, e1->value);
				moveNext(List1);
			} else {
				changeEntry(N, i, e2->col, e2->value);
			}
		}
		if (index(List1) == index(List2)) {		
			if (index(List1) > -1) {
				checkList = List1;
			} else {
				checkList = List2;
			}
			if (index(List1) > -1)
				s = 1;
			while(index(checkList) > -1) {
				Entry e3 = (Entry)get(checkList);
				changeEntry(N, i, e3->col, s * e3->value);
				moveNext(checkList);
			}
		}
	}	
	return N;
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
	if (size(A) != size(B)) {
		printf("Error in Matrix module: cannot call product() on two matrices of different sizes\n");
		exit(1);
	}
	Matrix M = transpose(B);
	Matrix N = newMatrix(size(A));
	for (int i = 1; i <= size(A); i++) {
		if (length(A->arr[i]) == 0) {
            for (int j = 1; j <= size(M); j++) {
                if (length(M->arr[j]) != 0) {
                    double prod = dotProduct(A->arr[i], M->arr[i]);
                    changeEntry(N, i, j, prod);
                }
            }
        }
	}
	return N;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M) {
	for (int i = 1; i <= size(M); i++) {
		if (length(M->arr[i]) > 0) {
			fprintf(out, "%d: ", i);
			moveFront(M->arr[i]);
			for (int j = 0; j < length(M->arr[i]); j++) {
				if (j != length(M->arr[i]) - 1)
					fprintf(out, "(%d, %.1f), ", ((Entry)get(M->arr[i]))->col, ((Entry)get(M->arr[i]))->value);
				else
					fprintf(out, "(%d, %.1f)", ((Entry)get(M->arr[i]))->col, ((Entry)get(M->arr[i]))->value);
				moveNext(M->arr[i]);
			}
			fprintf(out, "\n");
		}
	}
}
