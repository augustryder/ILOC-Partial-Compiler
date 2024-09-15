#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

/*
 * Sets the default options
 */
static void set_default_options(options_t* options) {
    options->help = false;
    options->debug = 0;
    options->use_colors = true; //Set this to false if colors are causing problems.
    options->verbose = false;
}

int debug_level = 0;

/*
 * Finds the matching case of the current command line option
 */
void switch_options (int arg, options_t* options) {
    switch (arg) {
        case 'h':
            options->help = true;
            help();
            exit(EXIT_SUCCESS);
            break;
        case 'd':
            options->debug= atoi(optarg);
            break;
        case 'v':
            options->verbose = true;
            break;
        case 0:
            options->use_colors = false;
            break;
        default:
            help();
            abort();
    }
}



/*
 * Tries to get the file name. Otherwise, gets stdin
 */
void get_file_name (int argc, char* argv[], options_t* options) {
    if (optind < argc) {
        strncpy(options->file_name, argv[optind++], FILE_NAME_SIZE);

    } else {
        strncpy(options->file_name, "-", FILE_NAME_SIZE);
    }
}


/*
 * Public function that loops until command line options were parsed
 */
void
options_parser (int argc, char* argv[], options_t* options)
{
    set_default_options(options);

    int arg; /* Current option */

    /* getopt allowed options */
    static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"debug", required_argument, 0, 'd'},
        {"no-colors", no_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
    };

    while (true) {

        int option_index = 0;
        arg = getopt_long(argc, argv, "hd:", long_options, &option_index);

        /* End of the options? */
        if (arg == -1) break;

        /* Find the matching case of the argument */
        switch_options(arg, options);
    }

    /* Gets the file name or exits with error */
    get_file_name(argc, argv, options);
    debug_level = options->debug;
}

void help() {
    fprintf(stdout, __PROGRAM_NAME__ "\n\n");
    fprintf(stdout, "Usage: ");
    fprintf(stdout, "%s [options] input file\n\n", __PROGRAM_NAME__);
    fprintf(stdout, "Options:\n\n");
    fprintf(stdout, "\t-dn, --version=-\n" 
                    "\t\tSets the debug level to n.\n\n");
    fprintf(stdout, "\t-h, --help\n" 
                    "\t\tPrints this help message\n\n");
    fprintf(stdout, "\t--no-color\n"
                    "\t\tDoes not use colors for printing\n\n");
    fprintf(stdout, "\t-v, --verbose\n"
                    "\t\tPrints list after every command\n\n");
}
