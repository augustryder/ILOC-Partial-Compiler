#pragma once

typedef struct edges {
    int label;
    struct edges* next;
} Neighbors;

typedef struct {
    int label;
    Neighbors* neighbors; // linked list of out-going neighbors
} Vertex;

typedef struct {
    int size;
    Vertex* vertices; // array of vertices
} Graph;

Neighbors* emptyNeighbors(void); // creates empty linked-list of neighbors
void freeNeighbors(Neighbors* nei);
Vertex initVertex(int label); // returns a vertex with no neighbors
Graph* initGraph(int size); // initializes a graph of 'size' vertices (labled 1..size) with no edges
void freeGraph(Graph* G);
void addEdge(int u, int v, Graph* G); // adds directed edge u -> v to graph G
void printGraph(Graph* G);




