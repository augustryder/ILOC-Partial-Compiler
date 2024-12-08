#include "graph.h"
#include <stdlib.h>
#include "utils.h"

Neighbors* emptyNeighbors(void) {
    Neighbors* nei = (Neighbors*) malloc(sizeof(Neighbors));
    assertCondition(nei, "Memory error allocating neighbors.");
    nei->label = -1;
    nei->next = NULL;
    return nei;
}

void freeNeighbors(Neighbors* nei) {
    Neighbors* rover = nei;
    Neighbors* nextNode;
    while (rover != NULL) {
        nextNode = rover->next;
        free(rover);
        rover = nextNode;
    }
}

Vertex initVertex(int label) {
    Vertex v = {label, -1, 0, -1, NULL, emptyNeighbors()};
    return v;
}

Graph* initGraph(int size) {
    Graph* g = (Graph*) malloc(sizeof(Graph));
    assertCondition(g, "Memory error allocating graph.");
    g->size = size;
    g->vertices = (Vertex*) malloc(sizeof(Vertex) * size);
    assertCondition(g->vertices, "Memory error allocating vertices.");
    for (int i = 0; i < size; ++i) {
        g->vertices[i] = initVertex(i+1);
    }
    return g;
}

void freeGraph(Graph* G) {
    for (int i = 0; i < G->size; ++i) {
        freeNeighbors(G->vertices[i].neighbors);
    }
    free(G->vertices);
    free(G);
}

void addEdge(int u, int v, Graph* G) {
    Neighbors* nei = G->vertices[u-1].neighbors;
    if (nei->label == -1) {
        nei->label = v;
    } else {
        Neighbors* newNei = emptyNeighbors();
        newNei->label = v;
        newNei->next = nei;
        G->vertices[u-1].neighbors = newNei;
    }
    G->vertices[v-1].indegree++;
}

void printGraph(Graph* G) {

    printf("nodes:\n");
    for (int i = 0; i < G->size; ++i) {
        Vertex v = G->vertices[i];
        printf("\tn%-3d: ", v.label);
        prettyPrintInst(v.inst, 1);
    }

    printf("edges:\n");
    for (int i = 0; i < G->size; ++i) {
        Vertex v = G->vertices[i];
        printf("\tn%-3d: ", v.label);
        Neighbors* nei = G->vertices[i].neighbors;
        if (nei->label == -1) {
            printf("{ }\n");
        } else {
            printf("{ ");
            Neighbors* rover = nei;
            while (rover != NULL) {
                printf("n%d", rover->label);
                rover = rover->next;
                if (rover != NULL) printf(", ");
            }
            printf(" }\n");
        }
    }

    printf("weights:\n");
    for (int i = 0; i < G->size; ++i) {
        Vertex v = G->vertices[i];
        printf("\tn%-3d: %d\n", v.label, v.distanceToRoot);
    }
    printf("\n");
}

