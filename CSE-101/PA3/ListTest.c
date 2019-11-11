#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.h"

int main(int argc, char* argv[]){
   
   List A = newList();
   List B = newList();
   List C = NULL;
   int i;

   append(A, 1);
   append(B, 1);
   append(B, 2);
   moveFront(A);
   moveFront(B);

   printList(stdout, A);
   printf("\n");
   printList(stdout, B);
   printf("\n");

   set(B, 2);
   printf("\n");
   printList(stdout, B);
   printf("\n");

   printList(stdout, copyList(B));
   return(0);
}

