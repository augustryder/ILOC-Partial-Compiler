#include "allocator.h"
#include "utils.h"

void computeLastUse(Block* block, Tables* tables) {
    int numLines = size(block);
    int maxSR = getMaxRegister(block);
    int inf = numLines * 2;

    tables->SRtoVR = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoVR != NULL, "Memory error allocating SRtoVR table.");

    tables->SRtoLU = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoLU != NULL, "Memory error allocating SRtoLU table.");
    
    tables->VRName = 0;
    for (int i = 0; i <= maxSR; ++i) {
        tables->SRtoVR[i] = -1;
        tables->SRtoLU[i] = inf;
    }
    Block* iter;
    for (int i = numLines - 1; i >= 0; --i) {
        update(iter->head->op3, i, tables);
        tables->SRtoVR[iter->head->op3->sr] = -1;
        tables->SRtoLU[iter->head->op3->sr] = inf;
        update(iter->head->op3, i, tables);
        update(iter->head->op3, i, tables);
        iter = iter->prev;
    }
}

static int update(Operand* OP, int index, Tables* tables) {
    if (tables->SRtoVR[OP->sr] == -1) {
        tables->SRtoVR[OP->sr] = tables->VRName++;
    }
    OP->vr = tables->SRtoVR[OP->sr];
    OP->nu = tables->SRtoLU[OP->sr];
    tables->SRtoLU[OP->sr] = index;
}