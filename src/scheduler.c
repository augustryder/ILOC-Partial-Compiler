#include "scheduler.h"
#include "allocator.h"
#include <stdlib.h>
#include "utils.h"

int latency(Inst* inst) {
    Opcode opcode = inst->opcode;
    if (opcode == LOAD || opcode == STORE) return 3;
    else if (opcode == MULT) return 2;
    else return 1;
}

Graph* buildDependencyGraph(Block* block, int blockSize) {
    // Initializes graph
    Tables tables;
    computeLastUse(block, blockSize, &tables);
    int maxVR = tables.VRName;
    Graph* G = initGraph(blockSize);

    int* VRtoVertex = (int*) malloc(sizeof(int) * (maxVR + 1)); // maps VR to it's definition vertex
    int lastOutput = -1; // vertex of most recent output
    int lastStore = -1; // vertext of most recent store
    Block* prevLoads = emptyBlock(); // block of all loads after the most recent store
    for (Block* rover = block; rover != NULL; rover = rover->next) {
        Inst* inst = rover->head;
        int u = inst->index + 1;
        G->vertices[u-1].inst = inst;
        G->vertices[u-1].latency = latency(inst);
        // Register edges
        if (inst->op1.vr != -1) {
            addEdge(u, VRtoVertex[inst->op1.vr], G);
        }
        if (inst->op2.vr != -1 && inst->op2.vr != inst->op1.vr) {
            addEdge(u, VRtoVertex[inst->op2.vr], G);
        }
        // Serialization edges
        switch (inst->opcode) {
            case OUTPUT:
                if (lastOutput != -1) addEdge(u, lastOutput, G);
                if (lastStore != -1)  addEdge(u, lastStore, G);
                lastOutput = u;
                break;
            case LOAD:
                if (lastStore != -1)  addEdge(u, lastStore, G);
                insert_at(prevLoads, inst, 0);
                break;
            case STORE:
                if (lastOutput != -1) addEdge(u, lastOutput, G);
                if (lastStore != -1)  addEdge(u, lastStore, G);
                // adds edges to previous loads after most recent store
                Block* load = prevLoads;
                Block* nextLoad;
                while (load != NULL && load->head != NULL) {
                    addEdge(u, load->head->index + 1, G);
                    nextLoad = load->next;
                    free(load);
                    load = nextLoad;
                }
                prevLoads = emptyBlock();
                lastStore = u;
                break;
            default:
                break;
        }
        // Assigns VR to a vertex
        if (inst->op3.vr != -1) {
            VRtoVertex[inst->op3.vr] = inst->index + 1;
        }
    }
    free(prevLoads);
    return G;
}

void dfs(Graph* G, int s, int* visited, Stack* order) {
    visited[s-1] = 1;
    Neighbors* neighbors = G->vertices[s-1].neighbors;
    for (Neighbors* nei = neighbors; nei != NULL && nei->label != -1; nei = nei->next) {
        if (visited[nei->label-1] == 0) {
            dfs(G, nei->label, visited, order);
        }
    }
    order->stack[--order->top] = s;
}

Graph* computeWeights(Graph* G) {
    // Compute topological ordering
    Stack order = {.size = G->size, 
                   .top = G->size, 
                   .stack = (int*) malloc(sizeof(int) * G->size)};
    int* visited = (int*) malloc(sizeof(int) * G->size);
    for (int i = 0; i < G->size; ++i) {
        Vertex* v = &(G->vertices[i]);
        if (v->indegree == 0) {
            v->distanceToRoot = v->latency; 
            dfs(G, v->label, visited, &order);
        }
    }
    // DAG relaxation
    for (int i = 0; i < order.size; ++i) {
        Vertex* u = &(G->vertices[order.stack[i]-1]);
        for (Neighbors* nei = u->neighbors; nei != NULL && nei->label != -1; nei = nei->next) {
            Vertex* v = &(G->vertices[nei->label-1]);
            if (u->distanceToRoot + v->latency > v->distanceToRoot) {
                v->distanceToRoot = u->distanceToRoot + v->latency;
            }
        }
    }
}


