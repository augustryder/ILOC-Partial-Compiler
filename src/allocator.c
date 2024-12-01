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
    op->ns = tables->lastStore;
    tables->SRtoLU[op->sr] = index;
}

// Annotates NUs, VRs, and returns MAXLIVE
int computeLastUse(Block* block, int blockSize, Tables* tables) {
    int inf = blockSize * 2;
    // Gets reversed list and gets max source register
    int maxSR = -1;
    Block* reversed = emptyBlock();
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        if (rover->head->op1.sr > maxSR) maxSR = rover->head->op1.sr;
        if (rover->head->op2.sr > maxSR) maxSR = rover->head->op2.sr;
        if (rover->head->op3.sr > maxSR) maxSR = rover->head->op3.sr;
        insert_at(reversed, rover->head, 0);
    }

    // Initialize tables
    tables->live = 0;
    tables->MAXLIVE = 0;
    tables->VRName = 0;
    tables->lastStore = inf;

    tables->SRtoVR = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoVR != NULL, "Memory error allocating SRtoVR table.");

    tables->SRtoLU = (int*) malloc(sizeof(int) * (maxSR + 1));
    assertCondition(tables->SRtoLU != NULL, "Memory error allocating SRtoLU table.");

    for (int i = 0; i <= maxSR; ++i) {
        tables->SRtoVR[i] = -1;
        tables->SRtoLU[i] = inf;
    }

    Block* iter = reversed;
    for (int i = blockSize - 1; i >= 0; --i) {
        Inst* inst = iter->head;
        if (inst->op3.sr != -1) {
            update(&inst->op3, i, tables);
            // Kill OP3
            if (tables->SRtoVR[inst->op3.sr] != -1) tables->live--;
            tables->SRtoVR[inst->op3.sr] = -1;
            tables->SRtoLU[inst->op3.sr] = inf;
        }
        if (inst->op1.sr != -1) update(&inst->op1, i, tables);
        if (inst->op2.sr != -1) update(&inst->op2, i, tables);
        if (inst->opcode == STORE) {
            tables->lastStore = i;
        }
        iter = iter->next;
    }

    // Check if there is a use without a definition
    if (tables->live > 0) {
        error("Use without a definition, undefined behavior.");
    }

    // Free reversed list
    while (reversed != NULL) {
        Block* nextNode = reversed->next;
        free(reversed);
        reversed = nextNode;
    }

    return tables->MAXLIVE;
}

// Helper function for localRegAlloc
static int getPR(Block** prevInstp, Stack* freePRs, Tables* tables) {
    Block* prevInst = *prevInstp;
    int index = prevInst->head->index + 1;
    int k = freePRs->size;
    // Gets PR from stack if non-empty, else spills PR
    if (freePRs->top < k) {
        int pr = freePRs->stack[freePRs->top++];
        // make sure OP2 doesn't choose pr if stack is empty
        tables->PRtoNU[pr] = -1;
        return pr;
    } else {
        // get furthest NU for all, clean, and rematerializable PRs
        int pr;
        int vr;
        int maxNU = 0;
        int maxClean = -1;
        int maxRemat = -1;
        for (int i = 0; i < k; ++i) {
            if (tables->PRtoNU[i] > index) {
                vr = tables->PRtoVR[i];
                if (tables->PRtoNU[i] > tables->PRtoNU[maxNU]) {
                    maxNU = i;
                }
                if (tables->VRtoML[vr] != -1) {
                    if (maxClean == -1 || tables->PRtoNU[i] > tables->PRtoNU[maxClean]) {
                        maxClean = i;
                    }
                } else if (tables->VRtoRM[vr] != -1) {
                    if (maxRemat == -1 || tables->PRtoNU[i] > tables->PRtoNU[maxRemat]) {
                        maxRemat = i;
                    }
                }
            }
        }

        // Heuristic for deciding spill
        int distToMaxNU = tables->PRtoNU[maxNU] - index;
        if (maxRemat >= 0 && tables->PRtoNU[maxRemat] > index + .20 * distToMaxNU) pr = maxRemat;
        else if (maxClean >= 0 && tables->PRtoNU[maxClean] > index + .50 * distToMaxNU) pr = maxClean;
        else pr = maxNU;

        vr = tables->PRtoVR[pr];

        // If VR is not rematerializable also isn't clean, insert spill instructions
        if (tables->VRtoRM[vr] == -1 && tables->VRtoML[vr] == -1) {
            // A loadI to put the spill location’s address into the reserved register
            // A store to move the spilled value from its PR into its spill location
            Operand op1 = {.val = tables->spillLoc, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
            Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
            Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1, .ns = -1};
            Inst* loadI = makeInst(LOADI, op1, op2, op3, -1);

            Operand op4 = {.val = -1, .sr = -1, .vr = vr, .pr = pr, .nu = -1, .ns = -1};
            Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1, .ns = -1};
            Operand op6 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
            Inst* store = makeInst(STORE, op4, op5, op6, -1);

            insert_after(prevInst, store);
            insert_after(prevInst, loadI);

            // Updates prevInst
            *prevInstp = prevInst->next->next;
            // Update table and memory location for pr's spill
            tables->VRtoML[vr] = tables->spillLoc;
            tables->spillLoc += 4;
        }

        // Kills VR to PR relation
        tables->isVRSpilled[vr] = 1;
        tables->VRtoPR[vr] = -1;
        tables->PRtoVR[pr] = -1;
        // make sure OP1 and OP2 don't both choose pr
        tables->PRtoNU[pr] = -1;

        return pr;
    }
}

void localRegAlloc(Block* block, int blockSize, int k) {
    int inf = blockSize * 2;
    Tables tables;
    // Computes MAXLIVE and reserves register if MAXLIVE > # PRS
    int MAXLIVE = computeLastUse(block, blockSize, &tables);
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

    tables.VRtoML = (int*) malloc(sizeof(int) * (tables.VRName + 1));
    assertCondition(tables.VRtoML != NULL, "Memory error allocating VRtoML table.");

    tables.VRtoRM = (int*) malloc(sizeof(int) * (tables.VRName + 1));
    assertCondition(tables.VRtoRM != NULL, "Memory error allocating VRtoRM table.");

    tables.isVRSpilled = (int*) malloc(sizeof(int) * (tables.VRName + 1));
    assertCondition(tables.isVRSpilled != NULL, "Memory error allocating isVRSpilled table.");

    tables.spillLoc = 32768;

    for (int i = 0; i <= tables.VRName; ++i) {
        tables.VRtoPR[i] = -1;
        tables.VRtoML[i] = -1;
        tables.VRtoRM[i] = -1;
        tables.isVRSpilled[i] = 0;
    }
    for (int i = 0; i < k; ++i) {
        tables.PRtoVR[i] = -1;
        tables.PRtoNU[i] = inf;
    }

    // Local Register Allocation
    Block* prevInst = NULL;
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;

        // If LOADI then set OP3.VR to rematerializable
        if (inst->opcode == LOADI) {
            tables.VRtoRM[inst->op3.vr] = inst->op1.val;
        }

        // If LOAD and address is remateriablizable and no stores before NU, set OP2.vr to clean
        if (inst->opcode == LOAD && tables.VRtoRM[inst->op1.vr] != -1 && inst->op3.ns >= inst->op3.nu) {
            tables.VRtoML[inst->op3.vr] = tables.VRtoRM[inst->op1.vr];
        }

        // Assigns OP1.PR and OP2.PR
        Operand* op = &(inst->op1);
        for (int i = 0; i < 2; ++i) {
            if (op->vr != -1) {
                if (tables.VRtoPR[op->vr] == -1) {
                    // gets a PR
                    tables.VRtoPR[op->vr] = getPR(&prevInst, &freePRs, &tables);
                    // Restores VR if it is spilled
                    if (tables.isVRSpilled[op->vr]) {
                        // If VR is rematerializable then rematerialize, otherwise restore from memory
                        if (tables.VRtoRM[op->vr] != -1) {
                            // A loadI to rematerialize the VRs value into its new PR
                            Operand op1 = {.val = tables.VRtoRM[op->vr], .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
                            Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
                            Operand op3 = {.val = -1, .sr = -1, .vr = op->vr, .pr = tables.VRtoPR[op->vr], .nu = -1, .ns = -1};
                            Inst* loadI = makeInst(LOADI, op1, op2, op3, -2);

                            insert_after(prevInst, loadI);
                            prevInst = prevInst->next;

                        } else {
                            // A loadI to put the put location’s address into the reserved register
                            // A load to retrieve the spilled value from its spill location into its new PR
                            Operand op1 = {.val = tables.VRtoML[op->vr], .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
                            Operand op2 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
                            Operand op3 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1, .ns = -1};
                            Inst* loadI = makeInst(LOADI, op1, op2, op3, -2);

                            Operand op4 = {.val = -1, .sr = -1, .vr = -1, .pr = k, .nu = -1, .ns = -1};
                            Operand op5 = {.val = -1, .sr = -1, .vr = -1, .pr = -1, .nu = -1, .ns = -1};
                            Operand op6 = {.val = -1, .sr = -1, .vr = op->vr, .pr = tables.VRtoPR[op->vr], .nu = -1, .ns = -1};
                            Inst* load = makeInst(LOAD, op4, op5, op6, -2);

                            insert_after(prevInst, load);
                            insert_after(prevInst, loadI);
                            prevInst = prevInst->next->next;

                        }
                        tables.isVRSpilled[op->vr] = 0;
                    }
                }
                op->pr = tables.VRtoPR[op->vr];    
                tables.PRtoVR[op->pr] = op->vr;
                // Sets VR to dirty if it's from user mem and there is a store before it's NU
                if (tables.VRtoML[op->vr] < 32768 && (op->ns < op->nu || inst->opcode == STORE)) {
                    tables.VRtoML[op->vr] = -1;
                }
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
    for (int i = freePRs->top; i < k; ++i) {
        printf("%d ", freePRs->stack[i]);
    }
    printf("\n");

}

void freeTables(Tables* tables) {
    free(tables->SRtoVR);
    free(tables->SRtoLU);
    free(tables->VRtoPR);
    free(tables->PRtoVR);
    free(tables->PRtoNU);
    free(tables->VRtoML);
    free(tables->VRtoRM);
    free(tables->isVRSpilled);
}
