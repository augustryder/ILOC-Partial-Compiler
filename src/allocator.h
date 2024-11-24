#pragma once
#include "block.h"
#include "inst.h"

typedef struct {
    int live;
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

// Annotates VRs, NUs, and returns MAXLIVE
int computeLastUse(Block* block, Tables* tables);
// Bottom-up local register allocation
void localRegAlloc(Block* block, int k);
// Prints tables
void printTables(Tables* tables, Stack* freePRs, int k, int maxVR);
// Frees allocated tables
void freeTables(Tables* tables);


