#pragma once

#include "inst.h"
#include <stdbool.h>

typedef struct list
{
    Inst* head;
    struct list* next;
} List;

List* emptyList();
int size(List* lst);
bool isEmpty(List* lst);
void printList(List* lst);
void tPrintList(List* lst);
void prettyPrintList(List* lst);
void append(List* lst, Inst* inst);
void insert_after(List* lst, Inst* inst);
void insert_at(List* lst, Inst* inst, int idx);
void remove_next(List* lst);
void remove_at(List* lst, int idx);
void freeList(List* lst);
