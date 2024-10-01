#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "parse.h"


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
    tPrintList(IR);

    return EXIT_SUCCESS;
}
