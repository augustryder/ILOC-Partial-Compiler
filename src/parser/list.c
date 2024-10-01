#include <stdlib.h>
#include "list.h"
#include "utils.h"

List* emptyList() {
  List* empty = (List*) malloc(sizeof(List));
  assertCondition(empty != NULL, "Memory error allocating inst.");
  empty->head = NULL;
  empty->next = NULL;
  return empty;
}

int size(List* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  int size = 0;
  List* rover = lst;
  while (rover != NULL) {
    size += 1;
    rover = rover->next;
  }
  return size;
}

bool isEmpty(List* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  return lst->head == NULL;
}

void printList(List* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    printf("Empty List");
    return;
  }
  List* rover = lst;
  while (rover != NULL) {
    printInst(rover->head);
    rover = rover->next;
  }
  printf("\n");
}

void tPrintList(List* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    printf("Empty List");
    return;
  }
  printf("|    x     |    y     |\n");
  List* rover = lst;
  while (rover != NULL) {
    tPrintInst(rover->head);
    rover = rover->next;
  }
  printf("\n");
}

void append(List* lst, Inst* inst) {
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    lst->head = inst;
  }
  else {
    List* rover = lst;
    while (rover->next != NULL) {
        rover = rover->next;
    }
    List* newNode = emptyList();
    newNode->head = inst;
    rover->next = newNode;
  }
}

void insert_after(List* lst, Inst* inst){
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    error("Uh oh! You cannot insert_after on an empty list!");
  }
  List* newNode = emptyList();
  newNode->head = inst;
  if (lst->next == NULL) {
    lst->next = newNode;
  } else {
    List* tmp = lst->next;
    lst->next = newNode;
    newNode->next = tmp;
  }
}

void insert_at(List* lst, Inst* inst, int idx) {
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (idx > size(lst)) {
    error("Uh oh! Index out of bounds!");
  }
  List* newNode;
  if (idx == 0) {
    newNode = emptyList();
    newNode->head = lst->head;
    newNode->next = lst->next;
    lst->head = inst;
    lst->next = newNode;
  } else {
    List* rover = lst;
    for (int i = 0; i < idx-1; i++){
      rover = rover->next;
    }
    insert_after(rover, inst);
  }
}

void remove_next(List* lst) {
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst) || lst->next == NULL) {
    error("Uh oh! You cannot remove_next on an empty list, or at the end of a list!");
  }
  List* deadNode = lst->next;
  lst->next = deadNode->next;
  if(deadNode->head == NULL){
    error("FREEING NULL");
  }else{
  free(deadNode->head);
  }
  free(deadNode);
}

void remove_at(List* lst, int idx) {
  List* deadNode;
  assertCondition(lst != NULL, "Null Parameter.");
  if (idx >= size(lst)) {
    error("Uh oh! Index out of bounds!");
  }
  if (isEmpty(lst)) {
    error("Uh oh! Cannot remove_at from an empty list!");
  }
  if (idx == 0) {
    free(lst->head);
    if (lst->next == NULL) {
      lst->head = NULL;
    } else {
      lst->head = lst->next->head;
      lst->next->head = NULL;
      deadNode = lst->next;
      lst->next = deadNode->next;
      free(deadNode);
    }
  } else {
    List* rover = lst;
    for (int i = 0; i < idx-1; i++){
      rover = rover->next;
    }
    remove_next(rover);
  }
}


void freeList(List *lst) {
    List *rover = lst;
    List *nextNode;

    while (rover != NULL) {
        nextNode = rover->next;  // Save the next node

        if (rover->head != NULL) {
            free(rover->head);  // Free the dynamically allocated Inst* in each node
            rover->head = NULL;  // Avoid double free by setting to NULL
        }

        free(rover);             // Free the current List node
        rover = nextNode;        // Move to the next node
    }
}
