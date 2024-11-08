#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "parse.h"
#include "scanner.h"
#include "block.h"
#include "allocator.h"


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

    Block* block = parse(file);

    if (options.lexer) {
        printf("// LEXER OUTPUT\n");
        printf("// ------------\n");
        fseek(file, 0, SEEK_SET);
        printTokenStream(file);
        if (options.tablePrint) tPrintBlock(block);
        if (options.prettyPrint >= 0) prettyPrintBlock(block, options.prettyPrint);
    }

    if (options.alloc) {
        printf("// ALLOCATOR OUTPUT\n");
        printf("// ----------------\n");
        if (options.k >= 3) {
            localRegAlloc(block, options.k);
        } else {
            error("Allocator needs at least 3 registers.");
        }
        if (options.prettyPrint >= 0) prettyPrintBlock(block, options.prettyPrint);
        else prettyPrintBlock(block, 2); // default print PRS
        if (options.tablePrint) tPrintBlock(block);
    }
    
    freeBlock(block);
    fclose(file);
    return EXIT_SUCCESS;
}
