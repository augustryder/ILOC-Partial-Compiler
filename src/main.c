#include <stdio.h>
#include <stdlib.h>
#include "line.h"
#include "args.h"
#include "scanner.h"
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
    while (true) {
        char c = getc(pFile);
        if (c == EOF) break;
        ungetc(c, pFile);
        Token test = s0(pFile);
        printToken(test);
        skipWhitespace(pFile);
    }
    return EXIT_SUCCESS;
}
