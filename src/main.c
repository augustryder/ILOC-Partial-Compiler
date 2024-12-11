#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "parse.h"
#include "scanner.h"
#include "block.h"
#include "allocator.h"
#include "graph.h"
#include "scheduler.h"

int main(int argc, char* argv[]) {
    options_t options;
    options_parser(argc, argv, &options);

    FILE* file;
    if (strcmp(options.file_name, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(options.file_name, "rb");
        if (file == NULL) {
            error("Unable to open file.");
        }
    }

    int blockSize = 0;
    Block* block = parse(file, &blockSize);

    if (options.lexer) {
        fseek(file, 0, SEEK_SET);
        printTokenStream(file);
    }
    
    if (options.sched) {
        Graph* G = buildDependencyGraph(block, blockSize);
        computeWeights(G);
        printGraph(G);
        freeGraph(G);
    }

    if (options.alloc) {
        if (options.k >= 3) {
            localRegAlloc(block, blockSize, options.k);
        } else {
            error("Allocator needs at least 3 registers.");
        }
    }

    if (options.prettyPrint >= 0) prettyPrintBlock(block, options.prettyPrint);
    if (options.tablePrint) tPrintBlock(block);
    
    freeBlock(block);
    fclose(file);
    return EXIT_SUCCESS;
}
