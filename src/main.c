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
        fseek(file, 0, SEEK_SET);
        printTokenStream(file);
    }
    else if (options.prettyPrint == 1) prettyPrintBlock(block);
    else if (options.tablePrint) tPrintBlock(block);

    if (options.alloc) {
        localRegAlloc(block, 3);
    }
    if (options.prettyPrint == 1) prettyPrintBlock(block);
    else if (options.tablePrint) tPrintBlock(block);
    
    freeBlock(block);
    fclose(file);
    return EXIT_SUCCESS;
}
