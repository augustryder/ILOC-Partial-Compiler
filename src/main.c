#include <stdio.h>
#include <stdlib.h>
#include "parser/inst.h"
#include "args.h"
#include "lexer/scanner.c"
#include "lexer/token.c"
#include <string.h>

int main(int argc, char* argv[]) {
    options_t options;
    options_parser(argc, argv, &options);

    FILE* pFile;
    if (strcmp(options.file_name, "-") == 0) {
        pFile = stdin;
    } else {
        pFile = fopen(options.file_name, "rb");
    }

    while (peek(pFile) != EOF) {
        printToken(nextToken(pFile));
    }

    return EXIT_SUCCESS;
}
