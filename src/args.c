#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "utils.h"

/*
 * Sets the default options
 */
static void set_default_options(options_t* options) {
    options->lexer = false;
    options->alloc = false;
    options->prettyPrint = 0;
    options->tablePrint = false;
    options->help = false;
    options->debug = 0;
}

int debug_level = 0;

/*
 * Finds the matching case of the current command line option
 */
static void switch_options(int arg, options_t* options) {
    switch (arg) {
        case 'l':
            options->lexer = true;
            break;
        case 'a':
            options->alloc = true;
            break;
        case 'p':
            if (optarg) {
                options->prettyPrint = atoi(optarg);
            } else {
                error("Error: -p option requires an argument.");
            }
            break;
        case 't':
            options->tablePrint = true;
            break;
        case 'h':
            options->help = true;
            help();
            exit(EXIT_SUCCESS);
            break;
        case 'd':
            if (optarg) {
                options->debug = atoi(optarg);
            } else {
                error("Error: -d option requires an argument.");
            }
            break;
        default:
            help();
            abort();
    }
}

/*
 * Tries to get the file name. Otherwise, gets stdin
 */
static void get_file_name(int argc, char* argv[], options_t* options) {
    if (optind < argc) {
        strncpy(options->file_name, argv[optind++], FILE_NAME_SIZE-1);

    } else {
        strncpy(options->file_name, "-", FILE_NAME_SIZE-1);
    }
}

/*
 * Public function that loops until command line options were parsed
 */
void options_parser (int argc, char* argv[], options_t* options) {
    set_default_options(options);

    int arg; /* Current option */

    /* getopt allowed options */
    static struct option long_options[] =
    {
        {"lexer", no_argument, 0, 'l'},
        {"allocator", no_argument, 0, 'a'},
        {"pretty-print", required_argument, 0, 'p'},
        {"table-print", no_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {"debug", required_argument, 0, 'd'}
    };

    while (true) {

        int option_index = 0;
        arg = getopt_long(argc, argv, "lap:thd:", long_options, &option_index);

        /* End of the options? */
        if (arg == -1) break;

        /* Find the matching case of the argument */
        switch_options(arg, options);
    }

    /* Gets the file name or exits with error */
    get_file_name(argc, argv, options);
    debug_level = options->debug;
}

void help(void) {
    fprintf(stdout, __PROGRAM_NAME__ "\n\n");
    fprintf(stdout, "Usage: ");
    fprintf(stdout, "%s [options] input file\n\n", __PROGRAM_NAME__);
    fprintf(stdout, "Options:\n\n");
    fprintf(stdout, "\t-l, --lexer=-\n" 
                    "\t\tPrints out the stream of tokens.\n\n");
    fprintf(stdout, "\t-p, --pretty-print\n" 
                    "\t\tPrints legal ILOC code.\n\n");
    fprintf(stdout, "\t-t, --table-print\n" 
                    "\t\tPrints the intermediate representation in tabular form.\n\n");
    fprintf(stdout, "\t-dn, --version=-\n" 
                    "\t\tSets the debug level to n.\n\n");
    fprintf(stdout, "\t-h, --help\n" 
                    "\t\tPrints this help message.\n\n");
}
