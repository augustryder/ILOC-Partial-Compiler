#include <stdlib.h>
#include "block.h"
#include "utils.h"

Block* emptyBlock(void) {
  Block* empty = (Block*) malloc(sizeof(Block));
  assertCondition(empty != NULL, "Memory error allocating Block.");
  empty->head = NULL;
  empty->next = NULL;
  return empty;
}

int size(Block* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  int size = 0;
  Block* rover = lst;
  while (rover != NULL) {
    size += 1;
    rover = rover->next;
  }
  return size;
}

int getMaxRegister(Block* block) {
  int max = -1;
  Block* rover = block;
  while (rover != NULL) {
    if (rover->head->op1.sr > max) max = rover->head->op1.sr;
    if (rover->head->op2.sr > max) max = rover->head->op2.sr;
    if (rover->head->op3.sr > max) max = rover->head->op3.sr;
    rover = rover->next; 
  }
  return max;
}

bool isEmpty(Block* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  return lst->head == NULL;
}

void printBlock(Block* lst){
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    printf("Empty Block");
    return;
  }
  Block* rover = lst;
  while (rover != NULL) {
    printInst(rover->head);
    rover = rover->next;
  }
  printf("\n");
}

void tPrintBlock(Block* lst) {
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    printf("Empty Block");
    return;
  }
  printf("| %-6s | %-6s | %-51s || %-51s || %-51s |\n", "Index", "OP", "OP1", "OP2", "OP3");
  printf("| %-6s | %-6s | %-6s | %-6s | %-6s | %-6s | %-6s | %-6s |", " ", " ", "Val", "SR", "VR", "PR", "NU", "NS");
  printf("| %-6s | %-6s | %-6s | %-6s | %-6s | %-6s |", "Val", "SR", "VR", "PR", "NU", "NS");
  printf("| %-6s | %-6s | %-6s | %-6s | %-6s | %-6s |\n", "Val", "SR", "VR", "PR", "NU", "NS");
  printf("|--------|--------|--------|--------|--------|--------|--------|--------|"
                           "|--------|--------|--------|--------|--------|--------|"
                           "|--------|--------|--------|--------|--------|--------|\n");
  Block* rover = lst;
  while (rover != NULL) {
    tPrintInst(rover->head);
    rover = rover->next;
  }
  printf("\n");
}

void prettyPrintBlock(Block* lst, int printLevel) {
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    printf("Empty Block");
    return;
  }
  Block* rover = lst;
  while (rover != NULL) {
    prettyPrintInst(rover->head, printLevel);
    rover = rover->next;
  }
  printf("\n");
}

void append(Block* lst, Inst* inst) {
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    lst->head = inst;
  }
  else {
    Block* rover = lst;
    while (rover->next != NULL) {
        rover = rover->next;
    }
    Block* newNode = emptyBlock();
    newNode->head = inst;
    rover->next = newNode;
  }
}

void insert_after(Block* lst, Inst* inst){
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (isEmpty(lst)) {
    error("Uh oh! You cannot insert_after on an empty block!");
  }
  Block* newNode = emptyBlock();
  newNode->head = inst;
  if (lst->next == NULL) {
    lst->next = newNode;
  } else {
    Block* tmp = lst->next;
    lst->next = newNode;
    newNode->next = tmp;
  }
}

void insert_at(Block* lst, Inst* inst, int idx) {
  assertCondition(lst != NULL && inst != NULL, "Null Parameter.");
  if (idx >= size(lst)) {
    error("Uh oh! Index out of bounds!");
  }
  Block* newNode;
  if (idx == 0) {
    if (isEmpty(lst)) {
      lst->head = inst;
    } else {
      newNode = emptyBlock();
      newNode->head = lst->head;
      newNode->next = lst->next;
      lst->head = inst;
      lst->next = newNode;
    }
  } else {
    Block* rover = lst;
    for (int i = 0; i < idx-1; i++){
      rover = rover->next;
    }
    insert_after(rover, inst);
  }
}

void remove_next(Block* lst) {
  assertCondition(lst != NULL, "Null Parameter.");
  if (isEmpty(lst) || lst->next == NULL) {
    error("Uh oh! You cannot remove_next on an empty block, or at the end of a block!");
  }
  Block* deadNode = lst->next;
  lst->next = deadNode->next;
  free(deadNode);
}

void remove_at(Block* lst, int idx) {
  assertCondition(lst != NULL, "Null Parameter.");
  if (idx >= size(lst)) {
    error("Uh oh! Index out of bounds!");
  }
  if (isEmpty(lst)) {
    error("Uh oh! Cannot remove_at from an empty block!");
  }
  Block* deadNode;
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
    Block* rover = lst;
    for (int i = 0; i < idx-1; i++){
      rover = rover->next;
    }
    remove_next(rover);
  }
}


void freeBlock(Block *lst) {
    assertCondition(lst != NULL, "Null Paramter.");
    Block *rover = lst;
    Block *nextNode;
    while (rover != NULL) {
        nextNode = rover->next;  // Save the next node

        if (rover->head != NULL) {
            free(rover->head);  // Free the dynamically allocated Inst* in each node
            rover->head = NULL;  // Avoid double free by setting to NULL
        }

        free(rover);             // Free the current Block node
        rover = nextNode;        // Move to the next node
    }
}
