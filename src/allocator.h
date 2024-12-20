#pragma once
#include "block.h"
#include "inst.h"

typedef struct {
    int live;
    int MAXLIVE;
    int VRName;
    int lastStore;
    int* SRtoVR;
    int* SRtoLU;
    int* VRtoPR;
    int* PRtoVR;
    int* PRtoNU;
    int* VRtoML; // VR to it's memory location, if it's clean
    int* VRtoRM; // VR to it's rematerializable value
    int* isVRSpilled; // isVRSpilled[vr0] == 1 if vr0 is currently spilled, 0 otheriwse
    int spillLoc;
} Tables;

typedef struct {
    int size;
    int top;
    int* stack;
} Stack;

// Annotates VRs, NUs, and returns MAXLIVE
int computeLastUse(Block* block, int blockSize, Tables* tables);
// Bottom-up local register allocation
void localRegAlloc(Block* block, int blockSize, int k);
// Prints tables
void printTables(Tables* tables, Stack* freePRs, int k, int maxVR);
// Frees allocated tables
void freeTables(Tables* tables);


