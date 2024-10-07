#include "allocator.h"

void computeLastUse(Block* block, Tables* tables) {
    int numLines = size(block);
    int infinity = numLines * 2;
    int invalid = -1;
    int MAX_REG = tables->maxSR;
    int SRtoVR[MAX_REG];
    int SRtoLU[MAX_REG];
    tables->VRName = 0;
    tables->SRtoVR = SRtoVR;
    tables->SRtoLU = SRtoLU;
    for (int i = 0; i <= MAX_REG; ++i) {
        SRtoVR[i] = invalid;
        SRtoLU[i] = infinity;
    }
    Block* iter;
    for (int i = numLines - 1; i >= 0; --i) {
        Update(iter->head->op3, i, tables);
        SRtoVR[iter->head->op3->sr] = invalid;
        SRtoLU[iter->head->op3->sr] = infinity;
        Update(iter->head->op3, i, tables);
        Update(iter->head->op3, i, tables);
    }
}

int update(Operand* OP, int index, Tables* tables) {
    if (tables->SRtoVR[OP->sr] == -1) {
        tables->SRtoVR[OP->sr] = tables->VRName++;
    }
    OP->vr = tables->SRtoVR[OP->sr];
    OP->nu = tables->SRtoLU[OP->sr];
    tables->SRtoLU[OP->sr] = index;
}