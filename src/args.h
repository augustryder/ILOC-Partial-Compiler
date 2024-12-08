#pragma once

#include <stdbool.h>

/* Program name */
#define __PROGRAM_NAME__ "builder"
/* Max size of a file name */
#define FILE_NAME_SIZE 512
extern int debug_level;

/* Defines the command line allowed options struct */
struct options {
    bool lexer;
    bool alloc;
    bool sched;
    int k;
    int prettyPrint;
    bool tablePrint;
    bool help;
    int debug;
    char file_name[FILE_NAME_SIZE];
};

/* Exports options as a global type */
typedef struct options options_t;

/* Public functions section */
void options_parser(int argc, char* argv[], options_t* options);
void help(void);


