#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.h"

typedef struct NodeObj {
	void* data;
	struct NodeObj* next;
	struct NodeObj* prev;
} NodeObj;

typedef NodeObj* Node;

Node newNode(void* s) {
	Node N = malloc(sizeof(NodeObj));
	assert (N != NULL);
	N->data = s;
	N->next = NULL;
	N->prev = NULL;
	return N;
}

void freeNode(Node* pN) {
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

typedef struct ListObj {
	Node head;
	Node tail;
	Node cursor;
	int index; 		//default state for cursor index
	int numItems;
} ListObj;
	
List newList(void) {
	List L = malloc(sizeof(ListObj));
	assert (L != NULL);
	L->head = NULL;
	L->tail = NULL;
	L->cursor = NULL;
	L->index = -1;
	L->numItems = 0;
	return L;
}

void freeList(List* pL) {
	if (*pL != NULL && pL != NULL) {
		while ((*pL)->numItems != 0) { //iterate down the list and free the previous node
			deleteFront(*pL); //delete each element, deleteFront() should handle freeing
		}
		free(*pL); //free the pointer to the list
		*pL = NULL;
	}
}


// Access functions -----------------------------------------------------------
int length(List L) {
	if (L == NULL) {
		printf("Error in module List, length() cannot be called on a NULL object\n");
		exit(1);
	}
	return L->numItems;
}

int index(List L) {
	if (L == NULL) {
		printf("Error in module List, index() cannot be called on a NULL object\n");
		exit(1);
	}
	return (L->index);
}

void* front(List L) {
	if (L == NULL) {
		printf("Error in module List, front() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, front() cannot be called on an empty List\n");
	}
	return L->head->data;
} // Returns front element of L. Pre: length()>0

void* back(List L) {
	if (L == NULL) {
		printf("Error in module List, back() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, back() cannot be called on an empty List\n");
		exit(1);
	}
	return L->tail->data;
	
} // Returns back element of L. Pre: length()>0

void* get(List L){
	if (L == NULL) {
		printf("Error in module List, get() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->cursor == NULL) {
		printf("get() is being called when the cursor is NULL\n");
		exit(1);
	}
	if (L->index == -1) {
		printf("Error in module List, get() cannot be called on an invalid index\n");
		exit(1);
	}
	return L->cursor->data;
	
} 							// Returns cursor element of L. Pre: length()>0, index()>=0

// int equals(List A, List B) {
// 	if (A == NULL || B == NULL) {
// 		printf("Error in module List, equals() cannot be called on a NULL object(s)\n");
// 		exit(1);
// 	}
// 	if (A->numItems != B->numItems) { //check for numitems
// 		return 0;
// 	}
// 	Node tempA = A->head;
// 	Node tempB = B->head;
// 	while (tempA != NULL && tempB != NULL) { 
// 		if (tempA->data != tempB->data) //if current elements arent the same, break out and return false
// 			return 0;
// 		tempA = tempA->next;
// 		tempB = tempB->next;
// 	}
// 	return 1;
// } 							// Returns true (1) iff Lists A and B are in same
// 							// state, and returns false (0) otherwise.

// Manipulation procedures ----------------------------------------------------
void clear(List L) {
	if (L == NULL) {
		printf("Error in module List, clear() cannot be called on a NULL object\n");
		exit(1);
	}
	Node temp = L->head;
	while (temp != NULL) { //free each node between head and tail
		temp = temp->next;
	}
	L->numItems = 0;
	L->index = -1;
}							// Resets L to its original empty state.

void moveFront(List L) {
	if (L == NULL) {
		printf("Error in module List, moveFront() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems != 0) {
		L->index = 0;
		L->cursor = L->head;
	}
	
} 						// If L is non-empty, sets cursor under the front element,
						// otherwise does nothing.

void moveBack(List L) {
	if (L == NULL) {
		printf("Error in module List, moveBack() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems != 0) {
		L->index = L->numItems - 1;
		L->cursor = L->tail;
	}
} 	// If L is non-empty, sets cursor under the back element,
						// otherwise does nothing.

void movePrev(List L) {
	if (L == NULL) {
		printf("Error in module List, movePrev() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems != 0) {
		if (L->cursor != NULL && (L->cursor == L->head)) { //this direct comparison might not work
			L->cursor = NULL;
			L->index = -1;
		} else if (L->cursor != NULL) {
			L->cursor = L->cursor->prev;
			L->index--;
		}
	}
} 	// If cursor is defined and not at front, move cursor one
	// step toward the front of L; if cursor is defined and at
	// front, cursor becomes undefined; if cursor is undefined
	// do nothing

void moveNext(List L) {
	if (L == NULL) {
		printf("Error in module List, moveNext() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems != 0) {
		if (L->index == L->numItems - 1) {
			L->index = -1;
			L->cursor = NULL;
		} else if (L->cursor != NULL) {
			L->cursor = L->cursor->next;
			L->index++;
		}
	}
}				 // If cursor is defined and not at back, move cursor one
				 // step toward the back of L; if cursor is defined and at
				 // back, cursor becomes undefined; if cursor is undefined
				 // do nothing

void prepend(List L, void* data) {
	if (L == NULL) {
		printf("Error in module List, prepend() cannot be called on a NULL object\n");
		exit(1);
	}
	Node new = newNode(data);
	if (L->head == NULL) {
		L->head = L->tail = new;
	} else {
		L->head->prev = new;
		new->next = L->head; //connect the new node to head
		L->head = new; //make new head the new Node
		L->index++;
	}
	L->numItems++;
}								// Insert new element into L. If L is non-empty,
								// insertion takes place before front element.

void append(List L, void* data) {
	if (L == NULL) {
		printf("Error in module List, append() cannot be called on a NULL object\n");
		exit(1);
	}

	Node new = newNode(data);
	if (L->head != NULL) {
		L->tail->next = new;
		new->prev = L->tail;
		L->tail = new;
	} else {
		L->head = L->tail = new;
	}
	L->numItems++;
} 								// Insert new element into L. If L is non-empty,
								// insertion takes place after back element.

void insertBefore(List L, void* data) {
	if (L == NULL) {
		printf("Error in module List, insertBefore() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L-> numItems == 0) {
		printf("Error in module List, insertBefore() cannot be called on an empty list\n");
		exit(1);
	}
	if (L->index < 0) {
		printf("Error in module List, insertBefore() cannot be called on an undefined cursor index\n");
		exit(1);
	}
	Node N = newNode(data);
	if (L->cursor == L->head) {
		Node new = newNode(data);
		L->head->prev = new;
		new->next = L->head; //connect the new node to head
		L->head = new; //make new head the new Node
		L->index++;
	} else {
		Node temp = L->cursor->prev;
		L->cursor->prev = N;
		temp->next = L->cursor;
		L->index++;
	}
	L->numItems++;
	freeNode(&N);
}									// Insert new element before cursor.
									// Pre: length()>0, index()>=0

void insertAfter(List L, void* data) {
	if (L == NULL) {
		printf("Error in module List, insertAfter() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, insertAfter() cannot be called on an empty list\n");
		exit(1);
	}
	if (L->index < 0) {
		printf("Error in module List, insertAfter() cannot be called on an undefined cursor index\n");
		exit(1);
	}
	if (L->cursor == L->tail) {
		append(L, data);
	} else {
		Node N = newNode(data);
		N->prev = L->cursor;
		N->next = L->cursor->next;
		L->cursor->next->prev = N;
		L->cursor->next = N;
		L->numItems++;
	}
}									// Insert new element after cursor.
									// Pre: length()>0, index()>=0

void deleteFront(List L) {
	if (L == NULL) {
		printf("Error in module List, deleteFront() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, deleteFront() cannot be called on an empty list\n");
	}
	if (L->cursor == L->head) {
		L->index = -1;
		L->cursor = NULL;
		L->numItems--;
	} else {
		Node temp = L->head;
		L->head = L->head->next;
		freeNode(&temp);
		L->numItems--;
		L->index--;
	}
}	//Delete the front element. Pre: length()>0

void deleteBack(List L) {
	if (L == NULL) {
		printf("Error in module List, deleteBack() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, deleteBack() cannot be called on an empty list\n");
		exit(1);
	}
	if (L->numItems == 1) {
		clear(L);
	} else {
		if (L->cursor == L->tail) {
			L->cursor = NULL;
			L->index = -1;
		}
		Node N = L->tail;
	    L->tail = N->prev;
	    N->prev = NULL;
	    L->tail->next = NULL;
	    L->numItems--;
	    freeNode(&N);
	}

}   // Delete the back element. Pre: length()>0

void delete(List L) {
	if (L == NULL) {
		printf("Error in module List, delete() cannot be called on a NULL object\n");
		exit(1);
	}
	if (L->numItems == 0) {
		printf("Error in module List, delete() cannot be called on an empty list\n");
		exit(1);
	}
	if (L->index < 0) {
		printf("Error in module List, delete() cannot be called on an undefined cursor index\n");
		exit(1);
	}
    if(L->cursor == L->head)
    {	
    	Node temp = L->cursor;
    	L->head = L->head->next;
    	L->cursor = NULL;
    	L->index = -1;
    	L->numItems--;
    	freeNode(&temp);
	} else if (L->tail == L->cursor) {
		deleteBack(L);
	} else {
		Node temp = L->head;
		L->cursor->prev = L->cursor->next;
		L->cursor->next->prev = L->cursor->prev;
		freeNode(&temp);
		L->numItems--;
	}

	

} // Delete cursor element, making cursor undefined.
					 // Pre: length()>0, index()>=0

// Other operations -----------------------------------------------------------
	// void printList(FILE* out, List L) {
	// 	Node printer = L->head;
	// 	while (printer != NULL) {
	// 		fprintf(out, "%d ", printer->data);
	// 		printer = printer->next;
	// 	}

	// } // Prints to the file pointed to by out, a
	// 								   // string representation of L consisting
	// 								   // of a space separated sequence of integers,
	// 								   // with front on left.
	// List copyList(List L) {
	// 	List new = newList();
	// 	Node temp = L->head;
	// 	while (temp != NULL) {
	// 		append(new, temp->data);
	// 		temp = temp->next;
	// 	}
	// 	return new;
	// } // Returns a new List representing the same integer
	// 					   // sequence as L. The cursor in the new list is undefined,
	// 					   // regardless of the state of the cursor in L. The state
	// 					   // of L is unchanged.
