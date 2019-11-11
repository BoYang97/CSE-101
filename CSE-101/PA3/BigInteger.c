#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "List.h"
#include "BigInteger.h"

#define POWER 1
#define BASE 1000000000 //10 digits!
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
List normalizeList(List L);

typedef struct BigIntegerObj {
	int sign;
	List longList;
} BigIntegerObj;

// Constructors-Destructors ---------------------------------------------------
// newBigInteger()
// Returns a reference to a new BigInteger object in the zero state.
BigInteger newBigInteger() {
	BigInteger B = malloc(sizeof(BigIntegerObj));
	assert(B != NULL);
	B->sign = 0;
	B->longList = newList();
	return (B);
}
// freeBigInteger()
// Frees heap memory associated with *pN, sets *pN to NULL.
void freeBigInteger(BigInteger* pN) {
	if (*pN != NULL && pN != NULL) {
		freeList(&((*pN)->longList));
		free(pN);
		*pN = NULL;
	}
}
// Access functions -----------------------------------------------------------
// sign()
// Returns -1 if N is negative, 1 if N is positive, and 0 if N is in the zero
// state.
int sign(BigInteger N) {
	return (N->sign);
}
// compare()
// Returns -1 if A<B, 1 if A>B, and 0 if A=B.
int compare(BigInteger A, BigInteger B)  {
	if (A->sign == -1 && B->sign == 1) //A is negative, B is positive
		return -1;
	if (A->sign == 1 && B->sign == -1) //A is positive, B is negative
		return 1;
	if (length(A->longList) > length(B->longList) && A->sign == B->sign) //sign(A) == sign(B), if A is a longer number then it must be larger
		return 1;
	if (length(A->longList) < length(B->longList) && A->sign == B->sign) //sign(A) == sign(B), if A is a shorter number then it must be smaller
		return -1;
	if (length(A->longList) == length(B->longList)) { //actually compare the two numbers
		BigInteger Z = diff(A, B);
		if (Z->sign == -1)
			return -1;
		else
			return 1;
	}
	return 0;
}
// equals()
// Return true (1) if A and B are equal, false (0) otherwise.
int equals(BigInteger A, BigInteger B) {
    if (A->sign != B->sign)
        return 0;
	if (diff(A, B) == 0) {
	    return 1;
	} else {
	    return 1;
	}
}

// Manipulation procedures ----------------------------------------------------

// makeZero()
// Re-sets N to the zero state.
void makeZero(BigInteger N) {
	clear(N->longList);
	N->sign = 0;
}

// negate()
// Reverses the sign of N: positive <--> negative. Does nothing if N is in the
// zero state.
void negate(BigInteger N) {
	if (N->sign != 0)
		N->sign = -N->sign;
}

// BigInteger Arithmetic operations -----------------------------------------------

// stringToBigInteger()
// Returns a reference to a new BigInteger object representing the decimal integer
// represented in base 10 by the string s.
// Pre: s is a non-empty string containing only base ten digits {0,1,2,3,4,5,6,7,8,9}
// and an optional sign {+, -} prefix.
BigInteger stringToBigInteger(char* s) { //this is the function where we take in an array of chars and convert them to a bigint
	BigInteger B = newBigInteger();
	List L = newList();
	//handle + and - at beginning of char array
	int starting_iter = 1;
	char test[str__len(s) + 1];
	memset(test, 0, str__len(s) + 1);
	if (s[0] == '+') { //either explicitely positive, or implied 
		B->sign = 1;
		strcpy(test, s);
	} else if (s[0] == '-') { //explicitely negative
		B->sign = -1;
		strcpy(test, s); 
	} else { //implicitely positive
		test[0] = '+';
		B->sign = 1;
		strcat(test, s);
	}
	while (test[starting_iter] == '0') { //walk through leading zero's
		starting_iter++;
	}

	char temp[num_digits(BASE)];

	int b = strlen(test) - starting_iter - 1; //REMOVE -1 FOR STRING LITERALS
	int a = b - num_digits(BASE) + starting_iter;

	// printf("starting iter is: %d\n", starting_iter);
	// printf("Strlen is: %lu\n", strlen(s));
	// printf("B is : %d\n", b);
	// printf("S[b] is %c\n", s[b]);
	// printf("S[a] is: %c\n", s[a]);
	// printf("A is: %d\n", a);

	//NOTE: when there is no explicit +,- symbol something is off by 1... This seems to be the only problem//
	memset(temp, 0, sizeof(temp));

	while (a - starting_iter >= 0) { //THIS WORKS, HANDLE FRONT CONDITION
		int w = 0;
		for (int j = a; j <= b; j++) {
			temp[w] = test[j];
			w++;
		}
		temp[w] = '\0';
		long t = strtol(temp, NULL, 10); //not sure why this works... might be an issue in the future
		prepend(L, t);
		b -= num_digits(BASE);
		a -= num_digits(BASE);
	}

	int adjusted_length = str__len(test) - starting_iter - 1; //REMOVE -1 FOR STRING LITERALS
	if (adjusted_length % num_digits(BASE) > 0) {
		char temp2[adjusted_length % num_digits(BASE)];
		memset(temp, 0, sizeof(temp2));
		int i = 0;
		while (i < adjusted_length % num_digits(BASE)) {
			temp2[i] = test[i + starting_iter];
			i++;
		}
		temp2[i] = '\0';
		long t2 = strtol(temp2, NULL, 10);
		prepend(L, t2);
	}

	printf("\nPrinting BigInteger list: ");
	printList(stdout, L);
	printf("\n");

	B->longList = L;
	return (B);
}

int num_digits(int b) {
	int n = b;
	int count = 0;
    while(n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

int str__len(char *s) {
	int i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

// copy()
// Returns a reference to a new BigInteger object in the same state as N.
BigInteger copy(BigInteger N) {
	if (N->sign == 0 && length(N->longList) == 0)
		return newBigInteger();
	BigInteger B = newBigInteger();
	B->longList = copyList(N->longList);
	B->sign = N->sign;
	return B;
}

// add()
// Places the sum of A and B in the existing BigInteger S, overwriting its
// current state: S = A + B
void add(BigInteger S, BigInteger A, BigInteger B) {
	//check for A= S, B = S, A&B = S
	if (A == B) {
		B = copy(A);
	} 
	if (A == S) {
		A = copy(S);
	}
	if (B == S) {
		B = copy(S);
	}
	if (A->sign == 0 && B->sign != 0) { //A is in the empty state, B is not, no work to be done
		S = B;
		S->sign = B->sign;
	}
	else if (B->sign == 0 && A->sign != 0) { //B is in the empty state, A is not, no work to be done
		S = A;
		S->sign = A->sign;
	}
	else if (A->sign == 0 && B->sign == 0) { //Both A and B are in the empty state
		return;
	}
	else if (A->sign == 1 && B->sign == 1) { //neither A nor B is in empty state, and have the same sign
		List temp = newList();
		moveBack(A->longList);
		moveBack(B->longList);
		// POSSIBILITY FOR OFF BY ONE ERRORS >>>>>
		if (length(A->longList) == length(B->longList)) {
			while (index(A->longList) != -1) {
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
		}
		else if (length(A->longList) > length(B->longList)) {
			while (index(B->longList) != -1) { //maybe off by one?
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(A->longList) != -1) { //
				prepend(temp, get(A->longList));
				movePrev(A->longList);
			}
		}
		else { //(length(A->longList) < length(B->longList))
			while (index(A->longList) != -1) {
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(B->longList) != -1) {
				prepend(temp, get(B->longList));
				movePrev(B->longList);
			}
		}
		// IN THIS SECTION >>>>>>>
		// printf("PRINTING LIST BEFORE NORMALIZATION: ");
		// printList(stdout, temp);
		// printf("\n");
		List normList = normalizeList(temp); //let normalize handle formatting the new BigInteger, probably we need the bigmaints? 
		S->longList = normList;
		S->sign = A->sign;
	} else if (A->sign == -1 && B->sign == -1) { //-A + -B = -A-B = -(A+B)
		BigInteger A_COPY = copy(A);
		BigInteger B_COPY = copy(B);
		A_COPY->sign = B_COPY->sign = 1;
		S = sum(A_COPY, B_COPY);
		S->sign = -1;
	} else { //A and B do not have the same sign, length shouldnt matter because it will be handled by subtraction
		if (A->sign == -1 && B->sign == 1) { //-A+B = B-A
			printf("should be here in add\n");
			BigInteger A_COPY = copy(A);
			A_COPY->sign = 1;
			subtract(S, B, A_COPY); //check for S->sign??
		} else if (A->sign == 1 && B->sign == -1) { //A+ -B = A-B
			BigInteger B_COPY = copy(B);
			B_COPY->sign = 1;
			subtract(S, A, B_COPY);
		}
	}
}

// sum()
// Returns a reference to a new BigInteger object representing A + B.
BigInteger sum(BigInteger A, BigInteger B) {
	BigInteger S = newBigInteger();
	add(S, A, B);
	return (S);
}

// subtract()
// Places the difference of A and B in the existing BigInteger D, overwriting
// its current state: D = A - B
void subtract(BigInteger D, BigInteger A, BigInteger B) {
	List temp = newList();
	if (A == B) {
		B = copy(A);
	}
	if (A == D) {
		A = copy(D);
	}
	if (B == D) {
		B = copy(D);
	}
	if (A->sign == 0 && B->sign != 0) { //A is in zero state, B is not, no work to be done
		D = B;
		D->sign = B->sign;
	} else if (B->sign == 0 && A->sign != 0) { //B is in zero state, A is not, no work to be done
		D = A;
		D->sign = A->sign;
	} else if (A->sign == 0 && B->sign == 0){ //both in zero state
		return;
	} else if (A->sign == 1 && B->sign == 1) { //neither A and B are in the zero state, do work A-B

		moveBack(A->longList);
		moveBack(B->longList);

		if (length(A->longList) == length(B->longList)) {
			while (index(A->longList) != -1) {
				long t = get(A->longList) - get(B->longList);
				prepend(temp, t);
				movePrev(A->longList);
				movePrev(B->longList);
			}
		} else if (length(A->longList) > length(B->longList)) {

			while (index(B->longList) != -1) {
				long t = get(A->longList) - get(B->longList);
				prepend(temp, t);
				movePrev(A->longList);
				movePrev(B->longList); 
			}
			while (index(A->longList) != -1) {
				prepend(temp, get(A->longList));
				movePrev(A->longList);
			}
		} else {
			while (index(A->longList) != -1) {
				long t = get(A->longList) - get(B->longList);
				prepend(temp, t);
				movePrev(A->longList);
				movePrev(B->longList); 
			}
			while (index(B->longList) != -1) {
				prepend(temp, get(B->longList));
				movePrev(B->longList);
			}
		}
		List normList = normalizeList(temp);
		printf("TEMP LIST IS: \n");
		printList(stdout, temp);
		printf("\n");
		D->longList = normList;
		// moveFront(normList);
		// if (get(normList) < 0) {
		// 	long t = get(normList);
		// 	t *= -1;
		// 	set(normList, t);
		// 	D->sign = -1;
		// } else {
		// 	D->sign = 1;
		// }
		// freeList(&normList);
    } else if (A->sign == -1 && B->sign == -1) { //-A-(-B) = -A+B = B-A
    	BigInteger A_COPY = copy(A);
    	BigInteger B_COPY = copy(B);
    	A_COPY->sign = B_COPY->sign = 1;
    	subtract(D, B_COPY, A_COPY);
    	D->sign = -1;
    	printList(stdout, D->longList);
    } else if (A->sign == 1 && B->sign == -1) { //A-(-B) = A+B
    	BigInteger A_COPY = copy(A);
    	BigInteger B_COPY = copy(B);
    	A_COPY->sign = B_COPY->sign = 1;
    	add(D, B_COPY, A_COPY);
    } else { //-A-B = -(A+B)
		BigInteger A_COPY = copy(A);
    	BigInteger B_COPY = copy(B);
    	A_COPY->sign = B_COPY->sign = 1;
    	add(D, B_COPY, A_COPY);
    	D->sign = -1;
    }
}

List newZeroList() {
	List L = newList();
	prepend(L, 0);
	return (L);
}

// diff()
// Returns a reference to a new BigInteger object representing A - B.
BigInteger diff(BigInteger A, BigInteger B) {
	BigInteger S = newBigInteger();
	subtract(S, A, B);
	return (S);
}

// multiply()
// Places the product of A and B in the existing BigInteger P, overwriting
// its current state: P = A*B
void multiply(BigInteger P, BigInteger A, BigInteger B) {

}

// prod()
// Returns a reference to a new BigInteger object representing A*B
BigInteger prod(BigInteger A, BigInteger B) {
	BigInteger D = newBigInteger();
	multiply(D, A, B);
	return (D);
}

// Other operations -----------------------------------------------------------
// printBigInteger()
// Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N) {
	moveFront(N->longList);
	if (N->sign == -1)
		fprintf(out, "-");
	fprintf(out, "%ld", get(N->longList));
	moveNext(N->longList);
	while (index(N->longList) != -1) {
		int num_zeros = num_digits(BASE) - num_digits(get(N->longList));
		if (num_zeros > 0 && index(N->longList) != length(N->longList)) {
			fprintf(out, "%010ld", get(N->longList));
		} else {
			fprintf(out, "%ld", get(N->longList));
		}
		moveNext(N->longList);
	}
	fprintf(out, "\n");
}

List normalizeList(List L) { //biggest helper function
	if (length(L) == 0)
		printf("WARNING: Calling normalizeList() on an empty list, did you mean to do that?\n");
	moveBack(L);
	int carry = 0;
	int final_carry = 0;
	long t = get(L);
	while (index(L) != 0) {
		if (t > 10000000000) {
			do {
				t -= 10000000000;
			} while (t > 10000000000);
			carry = 1; // maybe carry++?
		}
		if (t < 0) {
			do {
				t += 10000000000;
			} while (t < 0);
			carry = -1; // maybe carry--?
		}
		if (index(L) == 1) 
			final_carry = carry;
		set(L, t);
		movePrev(L);
		t = get(L);
		t += carry;
		carry = 0;
	}
	moveFront(L);
	t = get(L);
	t += final_carry;
	set(L, t);
	if (t > 10000000000) {
		do {
			t -= 10000000000;
		} while (t > 10000000000);
		carry = 1; // maybe carry++?
		set(L, t);
		prepend(L, carry);
	}
	if (t < 0) {
		do {
			t += 10000000000;
		} while (t < 0);
		carry = -1; // maybe carry++? //THIS MAY BE AN ISSUE, SOMEHOW PASS THIS NEGATIVE ONTO THE PARENT BIGINT
		set(L, t);
		prepend(L, carry);
	}
	printf("LIST AFTER NORMALIZATION: ");
	printList(stdout, L);
	return (L);
}