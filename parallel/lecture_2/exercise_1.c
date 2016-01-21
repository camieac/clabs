/*  Author: Cameron A. Craig
 *  Course: Disributed and Parallel Technologies
 *  Lecturer: Hans-Wolfgang Loidl
 *  Date: 14/01/2016
 *
 *  Exercise 1
 *  Write a function node `*append(node *x, node *y)` that
 *  appends the elements of the the second list y to the end of the
 *  first list x (i.e. append([1,2],[3,4]) ==> [1,2,3,4]).
 */
#include <stdio.h>
#include <stdlib.h>

/* Types */
typedef struct _node{
  int value;
  struct _node *next;
} node;

typedef node *list;

/* Prototypes */
node *mkList(int len, int *arr);
node *append(node *x, node *y);
void printList(node *list);

node *mkList(int len, int *arr) {
  int i;
  node *curr, *last, *root;
  if (len>0) {
    last = (node*) malloc(1*sizeof(node));
    last->value = arr[0];
    root = last;
  } else {
    return NULL;
  }

  for (i=0; i<len-1; i++) {
    curr = (node*) malloc(1*sizeof(node));
    curr->value = arr[i+1];
    last->next = curr;
    last = curr;
  }
  last->next = NULL;
  return root;
}

node *append(node *x, node *y) {
  node *root = NULL;
  node *curr = NULL, *last = NULL;
  while (x != NULL) {
    curr = (node*) malloc(1*sizeof(node));
    *curr = *x;
    if (root == NULL) { root = curr; }
    x = x->next;
    if (last != NULL) last->next = curr;
    last = curr;
  }

  while (y != NULL) {
    curr = (node*) malloc(1*sizeof(node));
    *curr = *y;
    y = y->next;
    if (last != NULL) last->next = curr;
    last = curr;
  }
  curr->next=NULL;
  return root;
}


void printList(node *list){
  if (list == NULL) {
    printf("List at %p: NIL\n", list);
    return;
  } else {
    printf("List at %p: %d", list, list->value);
    list = list->next;
  }
  while (list != NULL) {
    printf(", %d", list->value);
    list = list->next;
  }
  printf("\n");
}

int main(){

  int first_array[4] = {0, 1, 2, 3};
  int second_array[4] = {4, 5, 6, 7};

  node *first_list = mkList(4, first_array);
  node *second_list = mkList(4, second_array);

  node *result = append(first_list, second_list);

  printList(first_list);
  printList(second_list);
  printList(result);
}
