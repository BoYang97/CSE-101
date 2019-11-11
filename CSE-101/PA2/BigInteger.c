#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "List.c"
#include "BigInteger.h"

// #define POWER 1
// #define BASE pow(10, POWER)
// #define MAX(x, y) (((x) > (y)) ? (x) : (y))
// #define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
	B->sign = NULL;
	B->longList = newList();
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
	return (B->sign);
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
		BigInteger B = diff(A, B);
		if (B->sign == -1)
			return -1;
		else
			return 1;
	}
	return 0;
}
// equals()
// Return true (1) if A and B are equal, false (0) otherwise.
int equals(BigInteger A, BigInteger B) {
	return (diff(A, B) == 0);
}

// Manipulation procedures ----------------------------------------------------

// makeZero()
// Re-sets N to the zero state.
void makeZero(BigInteger N) {
	clear(N->longList);
	N->sign = NULL;
}

// negate()
// Reverses the sign of N: positive <--> negative. Does nothing if N is in the
// zero state.
void negate(BigInteger N) {
	if (length(N->longList) > 0)
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
	//handle + and - at beginning of char array
	if (s[0] == '+' || s[0] != '-') { //either explicitely positive, or implied 
		B->sign = 1;
	} else (s[0] == '-') { //clearly negative
		B->sign = -1;
	}
	int back_of_s = strlen(s) - 1; //index at the back of the array s
	// for (int i = back_of_s; i >= 0; i -= BASE) {

	// }
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
	if (A->sign == 0 && B->sign != 0) { //A is in the empty state, B is not, no work to be done
		S = B
	}
	else if (B->sign == 0 && A->sign != 0) { //B is in the empty state, A is not, no work to be done
		S = A
	}
	else if (A->sign == 0 && B->sign == 0) { //Both A and B are in the empty state
		return;
	}
	else if (A->sign == B->sign && A->sign != 0 && B->sign != 0) { //neither A nor B is in empty state, and have the same sign 
		List temp = newList();
		moveBack(A->longList);
		moveBack(B->longList);
		// POSSIBILITY FOR OFF BY ONE ERRORS >>>>>
		if (length(A->longList) == length(B->longList)) {
			while (index(A->longList) != 0) {
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
		}
		else if (length(A->longList) > length(B->longList)) {
			int extraAdd = length(A->longList) - length(B->longList);
			while (index(B->longList) != 0) { //maybe off by one?
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(A->longList) != 0) { //
				prepend(temp, get(A->longList));
				movePrev(A->longList);
			}
		}
		else { //(length(A->longList) < length(B->longList))
			int extraAdd = length(B->longList) - length(A->longList);
			while (index(A) != 0) {
				long sumTemp = get(A->longList) + get(B->longList);
				prepend(temp, sumTemp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(B->longList) != 0) {
				prepend(temp, get(B->longList));
				movePrev(B->longList);
			}
		}
		// IN THIS SECTION >>>>>>>
		normalizeList(temp); //let normalize handle formatting the new BigInteger, probably we need the bigints? 
		S->longList;
		S->sign = A->sign;
	} else { //A and B do not have the same sign, length shouldnt matter because it will be handled by subtraction
		if (A->sign == -1 && B->sign == 1)
			S = diff(B, A);
		else
			S = diff(A, B);
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
	if (A->sign == 0 && B->sign != 0) { //A is in zero state, B is not, no work to be done
		D = B;
		D->sign = B->sign;
	}
	else if (B->sign == 0 && A->sign != 0) { //B is in zero state, A is not, no work to be done
		D = A;
		D->sign = A->sign;
	}
	else if (A->sign == 0 && B->sign == 0){ //both in zero state
		return;
	}
	else if (A->sign == 1 && B->sign == 1) { //neither A and B are in the zero state, do work A-B
		List tempList = newList();
		moveBack(A->longList);
		moveBack(B->longList);
		if (length(A->longList) == length(B->longList)) {
			while (index(A->longList) != 0) {
				long temp = get(A->longList) - get(B->longList)
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
		} else if (length(A->longList) > length(B->longList)) {
			while (index(B->longList) != 0) { //probably supposed to be -1.... test for this error first
				long temp = get(A->longList) - get(B->longList)
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(A->longList) != 0) {
				prepend(tempList, get(A->longList));
				movePrev(A->longList);
			}
		} else { //(length(B->longList) > length(A->longList)) 
			while (index(A->longList) != 0) {
				long temp = get(A->longList) - get(B->longList)
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(B->longList) != 0) {
				prepend(tempList, -(get(B->longList)));
				movePrev(B->longList);
			}
			D->sign = -1;
		}
		D
	}
	else if (A->sign == 1 && B->sign == -1) { //A-(-B) = A+B
		D = sum(A, B);
		D->sign = 1;
	} 
	else if (A->sign == -1 && B->sign == 1) { //-A-B = -(A+B)
		D = sum(A, B);
		D->sign = -1;
	} 
	else  { //(A->sign == -1 && B->sign == -1) ---> -A-(-B) == -A+B == B-A
		List tempList = newList();
		moveBack(A->longList);
		moveBack(B->longList);
		if (length(A->longList) == length(B->longList)) {
			while (index(A->longList) != 0) {
				long temp = get(B->longList) - get(A->longList);
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
		} else if (length(B->longList) > length(A->longList)) {
			while (index(A->longList) != 0) {
				long temp = get(B->longList) - get(A->longList);
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(B->longList) != 0) {
				prepend(tempList, get(B->longList));
				movePrev(B->longList); 
			}
		} else { //length(B->longList) < length(A->longList)
			while (index(B->longList) != 0) {
				long temp = get(B->longList) - get(A->longList);
				prepend(tempList, temp);
				movePrev(A->longList);
				movePrev(B->longList);
			}
			while (index(A->longList) != 0) { //pretty sure this is supposed to be -1 but we will see during testing....
				prepend(tempList, -(get(A->longList))); //if subtracting a longer list the first elements before the second list will be 0 - that element, or -that element
				movePrev(A->longList);
			}
		}
	} 
}

// diff()
// Returns a reference to a new BigInteger object representing A - B.
BigInteger diff(BigInteger A, BigInteger B) {
	BigInteger S = newBigInteger();
	diff(S, A, B);
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
	D = multiply(D, A, B);
	return (D);
}

// Other operations -----------------------------------------------------------
// printBigInteger()
// Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N) {

}

List normalizeList(List L) { //biggest helper function
	if (length(L) == 0)
		printf("WARNING: Calling normalizeList() on an empty list, did you mean to do that?\n");

}	