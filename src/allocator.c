#include "allocator.h"
#include "utils.h"
#include "block.h"
#include <stdlib.h>

// Helper function for computeLastUse
static void update(Operand* op, int index, Tables* tables) {
    if (tables->SRtoVR[op->sr] == -1) {
        tables->SRtoVR[op->sr] = tables->VRName++;
        tables->live++;
        if (tables->live > tables->MAXLIVE) {
            tables->MAXLIVE = tables->live;
        }
    }
    op->vr = tables->SRtoVR[op->sr];
    op->nu = tables->SRtoLU[op->sr];
    tables->SRtoLU[op->sr] = index;
}

// Annotates NUs, VRs, and returns MAXLIVE
int computeLastUse(Block* block, Tables* tables) {
    int numLines = size(block);
    int maxSR = getMaxRegister(block);
    int inf = numLines * 2;

    // Initialize tables
    tables->live = 0;
    tables->MAXLIVE = 0;
    tables->VRName = 0;

    tables->SRtoVR = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoVR != NULL, "Memory error allocating SRtoVR table.");

    tables->SRtoLU = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoLU != NULL, "Memory error allocating SRtoLU table.");

    for (int i = 0; i <= maxSR; ++i) {
        tables->SRtoVR[i] = -1;
        tables->SRtoLU[i] = inf;
    }

    // Gets reversed list
    Block* reversed = emptyBlock();
    Block* rover = block;
    for (int i = 0; i < numLines; ++i) {
        insert_at(reversed, rover->head, 0);
        rover = rover->next;
    }

    Block* iter = reversed;
    for (int i = numLines - 1; i >= 0; --i) {
        Inst* inst = iter->head;
        if (inst->op3.sr != -1) {
            update(&inst->op3, i, tables);
            if (tables->SRtoVR[inst->op3.sr] != -1) tables->live--;
            tables->SRtoVR[inst->op3.sr] = -1;
            tables->SRtoLU[inst->op3.sr] = inf;
        }
        if (inst->op1.sr != -1) update(&inst->op1, i, tables);
        if (inst->op2.sr != -1) update(&inst->op2, i, tables);
        iter = iter->next;
    }

    // Check if there is a use without a definition
    if (tables->live > 0) {
        error("Use without a definition, undefined behavior.");
    }

    // free reversed list
    while (reversed != NULL) {
        Block* nextNode = reversed->next;  // Save the next node
        free(reversed);             // Free the current Block node
        reversed = nextNode;        // Move to the next node
    }

    return tables->MAXLIVE;
}

// Helper function for localRegAlloc
static int getPR(Block** prevInstp, Stack* freePRs, Tables* tables) {
    Block* prevInst = *prevInstp;
    int k = freePRs->size;
    // Gets PR from stack if non-empty, else spills PR
    if (freePRs->top < k) {
        int x = freePRs->stack[freePRs->top++];
        // make sure OP2 doesn't choose x if stack is empty
        tables->PRtoNU[x] = -1;
        return x;
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
        Inst* loadI = makeInst(LOADI, op1, op2, op3, -1);

        Operand op4 = {.val = -1, .sr = -1, .vr = tables->PRtoVR[x], .pr = x, .nu = -1};
        Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
        Operand op6 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
        Inst* store = makeInst(STORE, op4, op5, op6, -1);

        insert_after(prevInst, store);
        insert_after(prevInst, loadI);

        // Updates prevInst
        *prevInstp = prevInst->next->next;
        // Update tables and memory location for x's spill
        tables->VRtoPR[tables->PRtoVR[x]] = -1;
        tables->VRtoSL[tables->PRtoVR[x]] = tables->spillLoc;
        tables->PRtoVR[x] = -1;
        tables->spillLoc += 4;
        // make sure OP1 and OP2 don't both choose x 
        tables->PRtoNU[x] = -1;

        return x;
    }
}

void localRegAlloc(Block* block, int k) {

    int numLines = size(block);
    int inf = numLines * 2;

    Tables tables;
    // Computes MAXLIVE and reserves register if MAXLIVE > # PRS
    int MAXLIVE = computeLastUse(block, &tables);
    if (MAXLIVE > k) k--;

    // Initialize freePRs
    Stack freePRs = {.size = k, .top = 0, .stack = (int*) malloc(sizeof(int) * k)};
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

    tables.spillLoc = 32768;

    for (int i = 0; i <= tables.VRName; ++i) {
        tables.VRtoPR[i] = -1;
        tables.VRtoSL[i] = -1;
    }
    for (int i = 0; i < k; ++i) {
        tables.PRtoVR[i] = -1;
        tables.PRtoNU[i] = inf;
    }

    // Local Register Allocation
    Block* prevInst = NULL;
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;

        // Assigns OP1.PR and OP2.PR
        Operand* op = &(inst->op1);
        for (int i = 0; i < 2; ++i) {
            if (op->vr != -1) {
                if (tables.VRtoPR[op->vr] == -1) {
                    // gets a PR
                    tables.VRtoPR[op->vr] = getPR(&prevInst, &freePRs, &tables);
                    // checks if VR has been spilt, if so then restores
                    if (tables.VRtoSL[op->vr] != -1) {
                        // RESTORE OP1.VR
                        // A loadI to put the spill location’s address into the reserved register
                        // A load to retrieve the spilled value from its spill location into its new PR
                        Operand op1 = {.val = tables.VRtoSL[op->vr], .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                        Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                        Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                        Inst* loadI = makeInst(LOADI, op1, op2, op3, -2);

                        Operand op4 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1};
                        Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1};
                        Operand op6 = {.val = -1, .sr = -1, .vr = op->vr, .pr = tables.VRtoPR[op->vr], .nu = -1};
                        Inst* load = makeInst(LOAD, op4, op5, op6, -2);

                        insert_after(prevInst, load);
                        insert_after(prevInst, loadI);

                        prevInst = prevInst->next->next;

                        tables.VRtoSL[op->vr] = -1;
                    }
                }
                op->pr = tables.VRtoPR[op->vr];    
                tables.PRtoVR[op->pr] = op->vr;    
            }
            op = &(inst->op2);
        }  
        
        // Frees OP1.PR and or OP2.PR if NU = inf, otherwise updates respective PRs NU
        op = &(inst->op1);
        for (int i = 0; i < 2; ++i) {
            if (op->vr != -1) {
                if (op->pr == -1) {
                    printf("Failed to assign PR to OP%d line %d", i+1, inst->index);
                    error("");
                }
                if (op->nu == inf) {
                    // push OP.PR onto freePRs
                    freePRs.stack[--freePRs.top] = op->pr;
                    tables.VRtoPR[op->vr] = -1;
                    tables.PRtoVR[op->pr] = -1;
                    tables.PRtoNU[op->pr] = inf;
                } else {
                    tables.PRtoNU[op->pr] = op->nu;
                }
            }
            op = &(inst->op2);
        }

        // Assigns OP3.PR
        if (inst->op3.vr != -1) {
            // gets a PR and updates
            if (tables.VRtoPR[inst->op3.vr] == -1) {
                tables.VRtoPR[inst->op3.vr] = getPR(&prevInst, &freePRs, &tables);
            }
            inst->op3.pr = tables.VRtoPR[inst->op3.vr];
            tables.PRtoVR[inst->op3.pr] = inst->op3.vr;
            // Frees OP3.PR if NU = inf, otherwise updates PRs NU
            if (inst->op3.nu == inf) {
                // push OP3.PR onto FreePRs
                freePRs.stack[--freePRs.top] = inst->op3.pr;
                tables.VRtoPR[inst->op3.vr] = -1;
                tables.PRtoVR[inst->op3.pr] = -1;
                tables.PRtoNU[inst->op3.pr] = inf;
            } else {
                tables.PRtoNU[inst->op3.pr] = inst->op3.nu;
            }
        }

        prevInst = rover;
    }

    // Free tables and freePRs
    free(freePRs.stack);
    freeTables(&tables);
}

void printTables(Tables* tables, Stack* freePRs, int k, int maxVR) {

    int flag = 1;
    printf("PRtoVR: ");
    for (int i = 0; i < k; ++i) {
        printf("%d ", tables->PRtoVR[i]);
        if (tables->PRtoVR[i] != -1) {
            if (tables->VRtoPR[tables->PRtoVR[i]] != i) {
                flag = 0;
            }
        }
    }
    printf("\n");
    if (flag == 0) {
        printf("PRtoVR and VRtoPR inconsistent!\n");
    }
    printf("\n");

    flag = 1;
    printf("VRtoPR: ");
    for (int i = 0; i <= maxVR; ++i) {
        printf("%d ", tables->VRtoPR[i]);
        if (tables->VRtoPR[i] != -1) {
            if (tables->PRtoVR[tables->VRtoPR[i]] != i) {
                flag = 0;
            }
        }
    }
    printf("\n");
    if (flag == 0) {
        printf("PRtoVR and VRtoPR inconsistent!\n");
    }
    printf("\n");

    printf("FreePRs: ");
    printf("Top: %d ", freePRs->top);
    for (int i = freePRs->top; i < k; ++i) {
        printf("%d ", freePRs->stack[i]);
    }
    printf("\n");

}

void freeTables(Tables* tables) {
    free(tables->SRtoLU);
    free(tables->SRtoVR);
    free(tables->VRtoPR);
    free(tables->PRtoVR);
    free(tables->PRtoNU);
    free(tables->VRtoSL);
}
