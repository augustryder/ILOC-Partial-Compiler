#pragma once
#include "graph.h"
#include "block.h"
#include "allocator.h"

int latency(Inst* inst);
Graph* buildDependencyGraph(Block* block, int blockSize); // builds dependecy graph from block of code
void dfs(Graph* G, int s, int* visited, Stack* order);
Graph* computeWeights(Graph* G); // takes a dependecy graph and computes latency weighted distance to root
