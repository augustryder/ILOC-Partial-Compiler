#pragma once
#include "block.h"
#include "inst.h"

typedef struct {
    int live;
    int MAXLIVE;
    int VRName;
    int* SRtoVR;
    int* SRtoLU;
    int* VRtoPR;
    int* PRtoVR;
    int* PRtoNU;
    int* VRtoSL;
    int spillLoc;
} Tables;

typedef struct {
    int size;
    int top;
    int* stack;
} Stack;

// Computes last use, returns MAXLIVE
int computeLastUse(Block* block, Tables* tables);
// locally allocates registers
void localRegAlloc(Block* block, int k);

void freeTables(Tables* tables);


