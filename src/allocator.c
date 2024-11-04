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

static int getPR(Block* prevInst, Stack* freePRs, Tables* tables) {
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

        // insert spill instructions for x
        // A loadI to put the spill location’s address into the reserved register
        // A store to move the spilled value from its PR into its spill location
        Operand op1 = {.val = tables->spillLoc, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
        Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
        Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
        Inst* loadI = makeInst(LOADI, op1, op2, op3);

        Operand op4 = {.val = -1, .sr = -1, .vr = tables->PRtoVR[x], .pr = x, .nu = -1};
        Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
        Operand op6 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
        Inst* store = makeInst(STORE, op4, op5, op6);

        insert_after(prevInst, store);
        insert_after(prevInst, loadI);

        // Update tables, mem loc for x's vr
        tables->VRtoPR[tables->PRtoVR[x]] = -1;
        tables->VRtoSL[tables->PRtoVR[x]] = tables->spillLoc;
        tables->PRtoVR[x] = -1;
        tables->spillLoc += 4;

        // make sure op1 and op2 don't both choose x 
        tables->PRtoNU[x] = -1;

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

    tables.spillLoc = 372768;

    for (int i = 0; i <= tables.VRName; ++i) {
        tables.VRtoPR[i] = -1;
        tables.VRtoSL[i] = -1;
    }
    for (int i = 0; i < k; ++i) {
        tables.PRtoVR[i] = -1;
        tables.PRtoNU[i] = -1;
    }

    Block* prevInst = NULL;
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;

        // Assign OP1.PR
        if (inst->op1.vr != -1) {
            if (tables.VRtoPR[inst->op1.vr] == -1) {
                tables.VRtoPR[inst->op1.vr] = getPR(prevInst, &freePRs, &tables);
                if (tables.VRtoSL[inst->op1.vr] != -1) {
                    // RESTORE op1.vr
                    // A loadI to put the spill location’s address into the reserved register
                    // A load to retrieve the spilled value from its spill location into its new PR
                    Operand op1 = {.val = tables.VRtoSL[inst->op1.vr], .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                    Inst* loadI = makeInst(LOADI, op1, op2, op3);

                    Operand op4 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                    Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op6 = {.val = -1, .sr = -1, .vr = -1, .pr = tables.VRtoPR[inst->op1.vr], .nu = -1};
                    Inst* load = makeInst(LOAD, op4, op5, op6);

                    insert_after(prevInst, loadI);
                    insert_after(prevInst, load);

                }
            }
            inst->op1.pr = tables.VRtoPR[inst->op1.vr];    
            tables.PRtoVR[inst->op1.pr] = inst->op1.vr;    
        }

        // Assign OP2.PR
        if (inst->op2.vr != -1) {
            if (tables.VRtoPR[inst->op2.vr] == -1) {
                tables.VRtoPR[inst->op2.vr] = getPR(prevInst, &freePRs, &tables);
                if (tables.VRtoSL[inst->op2.vr] != -1) {
                    // RESTORE op2.vr
                    // A loadI to put the spill location’s address into the reserved register
                    // A load to retrieve the spilled value from its spill location into its new PR
                    Operand op1 = {.val = tables.VRtoSL[inst->op2.vr], .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                    Inst* loadI = makeInst(LOADI, op1, op2, op3);

                    Operand op4 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                    Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                    Operand op6 = {.val = -1, .sr = -1, .vr = -1, .pr = tables.VRtoPR[inst->op2.vr], .nu = -1};
                    Inst* load = makeInst(LOAD, op4, op5, op6);

                    insert_after(prevInst, load);
                    insert_after(prevInst, loadI);
                }
            }
            inst->op2.pr = tables.VRtoPR[inst->op2.vr];
            tables.PRtoVR[inst->op2.pr] = inst->op2.vr;   
        }
        
        if (inst->op1.pr != -1) {
            if (inst->op1.nu == inf) {
                // push tables.VRtoPR[inst->op1.vr] onto FreePRs
                freePRs.stack[--freePRs.top] = tables.VRtoPR[inst->op1.vr];
                tables.PRtoVR[tables.VRtoPR[inst->op1.vr]] = -1;
                tables.VRtoPR[inst->op1.vr] = -1;
                tables.PRtoNU[inst->op1.pr] = inf;
            } else {
                tables.PRtoNU[inst->op1.pr] = inst->op1.nu;
            }
        }

        if (inst->op2.pr != -1) {
            if (inst->op2.nu == inf) {
                // push tables.VRtoPR[inst->op1.vr] onto FreePRs
                freePRs.stack[--freePRs.top] = tables.VRtoPR[inst->op2.vr];
                tables.PRtoVR[tables.VRtoPR[inst->op2.vr]] = -1;
                tables.VRtoPR[inst->op2.vr] = -1;
                tables.PRtoNU[inst->op2.pr] = inf;
            } else {
                 tables.PRtoNU[inst->op2.pr] = inst->op2.nu;
            }
        }

        if (inst->op3.vr != -1) {
            tables.VRtoPR[inst->op3.vr] = getPR(prevInst, &freePRs, &tables);
            inst->op3.pr = tables.VRtoPR[inst->op3.vr];
            tables.PRtoVR[inst->op3.pr] = inst->op3.vr;   
            tables.PRtoNU[inst->op3.pr] = inst->op3.nu;
        }
        
        prevInst = rover;
    }

    // free things
    free(freePRs.stack);
    freeTables(&tables);
}

void freeTables(Tables* tables) {
    free(tables->SRtoLU);
    free(tables->SRtoVR);
    free(tables->VRtoPR);
    free(tables->PRtoVR);
    free(tables->PRtoNU);
    free(tables->VRtoSL);
}
