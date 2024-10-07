#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "parse.h"
#include "scanner.h"
#include "block.h"


int main(int argc, char* argv[]) {
    options_t options;
    options_parser(argc, argv, &options);

    FILE* file;
    if (strcmp(options.file_name, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(options.file_name, "rb");
    }

    Block* IR = parse(file);

    if (options.lexer) {
        fseek(file, 0, SEEK_SET);
        printTokenStream(file);
    }
    else if (options.prettyPrint) prettyPrintBlock(IR);
    else if (options.tablePrint) tPrintBlock(IR);

    freeBlock(IR);
    return EXIT_SUCCESS;
}
