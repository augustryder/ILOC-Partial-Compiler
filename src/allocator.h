#pragma once
#include "list.h"
#include "inst.h"

// typedef struct {
        int maxSR;
        int VRNext;
//     int* SRtoVR;
//     int* SRtoLU;
//     int* VRtoPR;
// } Tables;

// Computes last use, returns SRtoLU
int* computeLastUse(List* insts, int max_reg);
// locally allocates registers, returns VRtoPR
int* localRegAlloc(List* insts, int k);

// make helpers static?
void update(int OP, int index, int SRtoVR[], int SRtoLU[]);
int getPR(Inst* inst, int freePRs[], int VRtoPR[]);

