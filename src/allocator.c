#include "allocator.h"
#include "utils.h"
#include "block.h"
#include <stdlib.h>

// helper update function for computeLastUse
static void update(Operand* OP, int index, Tables* tables) {
    if (OP->sr == -1) return;
    if (tables->SRtoVR[OP->sr] == -1) {
        tables->SRtoVR[OP->sr] = tables->VRName++;
        tables->live++;
    }
    OP->vr = tables->SRtoVR[OP->sr];
    OP->nu = tables->SRtoLU[OP->sr];
    tables->SRtoLU[OP->sr] = index;
}

int computeLastUse(Block* block, Tables* tables) {
    int numLines = size(block);
    int maxSR = getMaxRegister(block);
    int inf = numLines * 2;

    tables->SRtoVR = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoVR != NULL, "Memory error allocating SRtoVR table.");

    tables->SRtoLU = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoLU != NULL, "Memory error allocating SRtoLU table.");
    
    tables->live = 0;
    tables->MAXLIVE = 0;
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
        if (inst->op3.sr != -1) {
            if (tables->SRtoVR[inst->op3.sr] != -1) tables->live--;
            tables->SRtoVR[inst->op3.sr] = -1;
            tables->SRtoLU[inst->op3.sr] = inf;
        }
        update(&inst->op1, i, tables);
        update(&inst->op2, i, tables);
        if (tables->live > tables->MAXLIVE) {
            tables->MAXLIVE = tables->live;
        }
        iter = iter->next;
    }

    // free reversed list
    while (reversed != NULL) {
        Block* nextNode = reversed->next;  // Save the next node
        free(reversed);             // Free the current Block node
        reversed  = nextNode;        // Move to the next node
    }

    return tables->MAXLIVE;
}

static int getPR(Inst* inst, Stack* freePRs, Tables* tables) {
    int k = freePRs->size;
    if (freePRs->top < k) {
        return freePRs->stack[freePRs->top++];
    } else {
        // get PR with furthest NU
        int x = 0;
        for (int i = 0; i < k; ++i) {
            if (tables->PRtoNU[i] > tables->PRtoNU[x]) {
                x = i;
            }
        }
        // insert spill isntructions for x
        // Update tables, mem loc for x, and tag x as spilt
        return x;
    }
}

void localRegAlloc(Block* block, int k) {
    Tables tables;

    int numLines = size(block);
    int inf = numLines * 2;

    int MAXLIVE = computeLastUse(block, &tables);
    if (MAXLIVE > k) k--;

    // Initialize freePRs
    Stack freePRs;
    freePRs.size = k;
    freePRs.top = 0;
    freePRs.stack = (int*) malloc(sizeof(int) * k);
    for (int i = 0; i < k; ++i) {
        freePRs.stack[i] = i;
    }

    // Initialize tables
    tables.VRtoPR = (int*) malloc(sizeof(int) * (tables.VRName + 1));
    assertCondition(tables.VRtoPR != NULL, "Memory error allocating VRtoPR table.");

    tables.PRtoVR = (int*) malloc(sizeof(int) * k);
    assertCondition(tables.PRtoVR != NULL, "Memory error allocating PRtoVR table.");

    tables.PRtoNU = (int*) malloc(sizeof(int) * k);
    assertCondition(tables.PRtoNU != NULL, "Memory error allocating PRtoNU table.");

    tables.VRtoSL = (int*) malloc(sizeof(int) * (tables.VRName + 1));
    assertCondition(tables.VRtoSL != NULL, "Memory error allocating VRtoSL table.");

    for (int i = 0; i <= tables.VRName; ++i) {
        tables.VRtoPR[i] = -1;
        tables.VRtoSL[i] = -1;
    }
    for (int i = 0; i < k; ++i) {
        tables.PRtoVR[i] = -1;
        tables.PRtoNU[i] = -1;
    }

    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;

        // Assign OP1.PR
        if (tables.VRtoPR[inst->op1.vr] == -1) {
            tables.VRtoPR[inst->op1.vr] = getPR(inst, &freePRs, &tables);
            if (tables.VRtoSL[inst->op1.vr] != -1) {
                // RESTORE op1.vr
            }
        }
        inst->op1.pr = tables.VRtoPR[inst->op1.vr];

        // Assign OP2.PR
        if (tables.VRtoPR[inst->op2.vr] == -1) {
            tables.VRtoPR[inst->op2.vr] = getPR(inst, &freePRs, &tables);
            if (tables.VRtoSL[inst->op2.vr] != -1) {
                // RESTORE op2.vr
            }
        }
        inst->op2.pr = tables.VRtoPR[inst->op2.vr];

        
        if (inst->op1.nu == inf) {
            // push tables.VRtoPR[inst->op1.vr] onto FreePRs
            tables.VRtoPR[inst->op1.vr] = -1;
        }
        tables.PRtoNU[inst->op1.pr] = inst->op1.nu;

        if (inst->op2.nu == inf) {
            // push tables.VRtoPR[inst->op1.vr] onto FreePRs
            tables.VRtoPR[inst->op2.vr] = -1;
        }
        tables.PRtoNU[inst->op2.pr] = inst->op2.nu;

        tables.VRtoPR[inst->op3.vr] = getPR(inst, &freePRs, &tables);
        inst->op3.pr = tables.VRtoPR[inst->op3.vr];
        tables.PRtoNU[inst->op3.pr] = inst->op3.nu;
    }

    freeTables(&tables);
}

void freeTables(Tables* tables) {
    free(tables->SRtoLU);
    free(tables->SRtoVR);
    free(tables->VRtoPR);
    free(tables->VRtoPR);
    free(tables->PRtoNU);
    free(tables->VRtoSL);
}
