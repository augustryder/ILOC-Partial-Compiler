#pragma once

#include "inst.h"
#include <stdbool.h>

typedef struct block
{
    Inst* head;
    struct block* next;
} Block;

Block* emptyBlock(void);
int size(Block* lst);
bool isEmpty(Block* lst);
void printBlock(Block* lst);
void tPrintBlock(Block* lst);
void prettyPrintBlock(Block* lst, int printLevel);
void append(Block* lst, Inst* inst);
void insert_after(Block* lst, Inst* inst);
void insert_at(Block* lst, Inst* inst, int idx);
void remove_next(Block* lst);
void remove_at(Block* lst, int idx);
void freeBlock(Block* lst);
