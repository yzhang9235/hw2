#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <list.h>
#include <string.h>
#include <table.h>
#include <atom.h>
#include <row.h>
#include <seq.h>
#include <ctype.h>
#include <pnmrdr.h>

int main (int argc, char *argv[]) 
{
        FILE *input;

        /* process command-line arguments */
        if (argc > 2) {
                fprintf(stderr, "too many arguments\n");
                exit(EXIT_FAILURE);
        } else if (argc == 1) {
                /* read from standard input */
                input = stdin;
        } else if (argc == 2) {
                /* open the given file */
                input = fopen(argv[1], "rb"); 
                /* CRE: file must be accessible */ 
                if (input == NULL) {
                        fprintf(stderr, "cannot open file '\n");
                        exit(EXIT_FAILURE);
                }
        }
}