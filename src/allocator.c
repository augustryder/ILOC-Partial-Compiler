#include "allocator.h"
#include "utils.h"
#include "block.h"
#include <stdlib.h>

// helper update function for computeLastUse
static void update(Operand* OP, int index, Tables* tables) {
    if (OP->sr == -1) return;
    if (tables->SRtoVR[OP->sr] == -1) {
        tables->SRtoVR[OP->sr] = tables->VRName++;
    }
    OP->vr = tables->SRtoVR[OP->sr];
    OP->nu = tables->SRtoLU[OP->sr];
    tables->SRtoLU[OP->sr] = index;
}

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

    // lazy way to get reversed list
    Block* reversed = emptyBlock();
    Block* rover = block;
    for (int i = 0; i < numLines; ++i) {
        insert_at(reversed, rover->head, 0);
        rover = rover->next;
    }
    // 
    Block* iter = reversed;
    for (int i = numLines - 1; i >= 0; --i) {
        Inst* inst = iter->head;
        update(&inst->op3, i, tables);
        tables->SRtoVR[inst->op3.sr] = -1;
        tables->SRtoLU[inst->op3.sr] = inf;
        update(&inst->op1, i, tables);
        update(&inst->op2, i, tables);
        iter = iter->next;
    }

    while (reversed != NULL) {
        Block* nextNode = reversed->next;  // Save the next node
        free(reversed);             // Free the current Block node
        reversed  = nextNode;        // Move to the next node
    }
}

