#pragma once
#include "block.h"
#include "inst.h"

typedef struct {
    int maxSR;
    int VRName;
    int* SRtoVR;
    int* SRtoLU;
    int* VRtoPR;
} Tables;

// Computes last use
void computeLastUse(Block* insts, Tables* tables);
// locally allocates registers
void localRegAlloc(Block* insts, int k, Tables* tables);

// make helpers static?
int getMaxRegister(Block* insts);
void update(Operand* OP, int iter, Tables* tables);
int getPR(Inst* inst, Tables* tables);

