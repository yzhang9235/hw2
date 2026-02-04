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
        //check input
        Pnmrdr_T reader = Pnmrdr_new(input);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);
        if (data.type != Pnmrdr_gray) {
                fprintf(stderr, "image is not a graymap\n");
                exit(EXIT_FAILURE);
        } else if (data.denominator != 9) {
                fprintf(stderr, "invalid denominator\n");
                exit(EXIT_FAILURE);
        }
        
        UArray2_T data_arr = UArray2_new(data.width, data.height, sizeof(int));
        UArray2_map_row_major(data_arr, populate_data, &reader);
        

}

void populate_data(int col, int row, T uarray2, void *p1, void *p2) 
{
        int *entry_p = p1;
        int *reader_p = p2;
        entry_p = Pnmrdr_get(reader_p);
        UArray_at(uarray2, col, row) = entry_p;
        printf("%d\n", UArray_at(uarray2, col, row));
}