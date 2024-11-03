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
} Tables;

// Computes last use, returns MAXLIVE
int computeLastUse(Block* insts, Tables* tables);
// locally allocates registers
void localRegAlloc(Block* insts, int k);

void freeTables(Tables* tables);
// make helpers static?
int getMaxRegister(Block* insts);
int getPR(Inst* inst, Tables* tables);

