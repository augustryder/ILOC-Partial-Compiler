#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "parse.h"
#include "scanner.h"
#include "list.h"


int main(int argc, char* argv[]) {
    options_t options;
    options_parser(argc, argv, &options);

    FILE* file;
    if (strcmp(options.file_name, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(options.file_name, "rb");
    }

    List* IR = parse(file);

    if (options.lexer) {
        fseek(file, 0, SEEK_SET);
        printTokenStream(file);
    }
    if (options.prettyPrint) prettyPrintList(IR);
    if (options.tablePrint) tPrintList(IR);

    return EXIT_SUCCESS;
}
