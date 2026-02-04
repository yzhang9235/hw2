#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <table.h>
#include <atom.h>
#include <ctype.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "uarray2.h"
#include "sudoku.h"



struct sudokuInfo {
        Table_T table;
        bool invalid_sudoku;
};

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
        /* initialize a uarray2*/
        UArray2_T data_arr = UArray2_new(data.width, data.height, sizeof(int));
        /* populate uarray2 with the data from the file */
        UArray2_map_row_major(data_arr, populate_data, &reader);


        /* Check whether there is no two pixels have the same intensity in one row*/
        struct sudokuInfo info;
        info.table = Table_new(9, NULL, NULL);
        info.invalid_sudoku = false;
        UArray2_map_row_major(data_arr, validate_row, &info);
        printf("%d\n", info.invalid_sudoku);
        UArray2_map_col_major(data_arr, validate_col, &info);
        printf("%d\n", info.invalid_sudoku);
        Table_free(&(info.table));

        UArray2_free(&data_arr);
        Pnmrdr_free(&reader);
        fclose(input);
        return EXIT_SUCCESS;

}

void populate_data(int col, int row, UArray2_T uarray2, void *p1, void *p2) 
{
        (void) col;
        (void) row;
        (void) uarray2;
        int *entry_p = p1;
        Pnmrdr_T *reader_p = p2;
        *entry_p = Pnmrdr_get(*reader_p);
}

bool is_correct_intensity(int pixel)
{
        return pixel > 0 && pixel < 10;
}

void validate_row(int col, int row, UArray2_T uarray2, void *p1, void *info_p)
{
        (void) row;
        (void) uarray2;

        int *entry_p = p1;
        //change the type to atom_int in order to put in table as a key
        const void *key = Atom_int(*entry_p);
        struct sudokuInfo *info = info_p;
        
        if (col == 0) {
                Table_free(&(info->table));
                info->table = Table_new(9, NULL, NULL);
        }

        if (Table_get(info->table, key) != NULL) {
                info->invalid_sudoku = true;
        } else {
                if (is_correct_intensity(*entry_p)) {
                        //only care whether key exists,
                        //we don't care value
                        Table_put(info->table, key, NULL);
                } else {
                        info->invalid_sudoku = true;
                        Table_put(info->table, key, NULL);
                }
                
        }
}

void validate_col(int col, int row, UArray2_T uarray2, void *p1, void *info_p)
{
        (void) col;
        (void) uarray2;

        int *entry_p = p1;
        //change the type to atom_int in order to put in table as a key
        const void *key = Atom_int(*entry_p);
        struct sudokuInfo *info = info_p;
        
        if (row == 0) {
                Table_free(&(info->table));
                info->table = Table_new(9, NULL, NULL);
        }

        if (Table_get(info->table, key) != NULL) {
                info->invalid_sudoku = true;
        } else {
                if (is_correct_intensity(*entry_p)) {
                        Table_put(info->table, key, NULL);
                } else {
                        info->invalid_sudoku = true;
                        Table_put(info->table, key, NULL);
                }
                
        }
}