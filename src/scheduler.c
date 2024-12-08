#include "scheduler.h"
#include "allocator.h"
#include <stdlib.h>
#include "utils.h"

Graph* buildDependencyGraph(Block* block, int blockSize) {
    // Initializes graph
    Tables tables;
    computeLastUse(block, blockSize, &tables);
    int maxVR = tables.VRName;
    Graph* G = initGraph(blockSize);

    int* VRtoVertex = (int*) malloc(sizeof(int) * (maxVR + 1)); // maps VR to it's definition vertex
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;
        if (inst->op1.vr != -1) {
            addEdge(inst->index + 1, VRtoVertex[inst->op1.vr], G);
        }
        if (inst->op2.vr != -1 && inst->op2.vr != inst->op1.vr) {
            addEdge(inst->index + 1, VRtoVertex[inst->op2.vr], G);
        }
        if (inst->op3.vr != -1) {
            VRtoVertex[inst->op3.vr] = inst->index + 1;
        }
    }
    return G;
}


