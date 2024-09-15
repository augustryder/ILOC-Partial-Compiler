#include <stdio.h>
#include <stdlib.h>
#include "line.h"
#include "args.h"

int main(int argc, char* argv[]) {
    options_t options;
    options_parser(argc, argv, &options);
    
    return EXIT_SUCCESS;
}
