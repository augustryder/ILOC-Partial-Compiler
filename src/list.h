#pragma once

#include "line.h"
#include <stdbool.h>

typedef struct list
{
    Line* head;
    struct list* next;
} List;

List* emptyList();
int size(List* lst);
bool isEmpty(List* lst);
void printList(List* lst);
void tPrintList(List* lst);
void append(List* lst, Line* line);
void insert_after(List* lst, Line* line);
void insert_at(List*lst, Line* line, int idx);
void remove_next(List* lst);
void remove_at(List* lst, int idx);
void freeList(List* lst);
