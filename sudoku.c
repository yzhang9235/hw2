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
        } else if (data.width != 9 || data.height != 9) {
                fprintf(stderr, "invalid width and height\n");
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
        
        UArray2_map_col_major(data_arr, validate_col, &info);

        validate_submaps(data_arr, &info);
        
        printf("%d\n", info.invalid_sudoku); //TEST

        UArray2_free(&data_arr);
        Pnmrdr_free(&reader);
        fclose(input);
        
        if (info.invalid_sudoku == true) {
                exit(EXIT_FAILURE);
        }
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
                        Table_put(info->table, key, entry_p);
                } else {
                        info->invalid_sudoku = true;
                        Table_put(info->table, key, entry_p);
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
                        Table_put(info->table, key, entry_p);
                } else {
                        info->invalid_sudoku = true;
                        Table_put(info->table, key, entry_p);
                }
                
        }
}

void validate_one_submap(int start_col, int start_row, UArray2_T map, void *info_p)
{
        struct sudokuInfo *info = info_p;

        for (int c = start_col; c < start_col + 3; c++) {
                for (int r = start_row; r < start_row + 3; r++) {
                        int *entry_p = UArray2_at(map, c, r);
                        const void *key = Atom_int(*entry_p);
                        if (Table_get(info->table, key) != NULL) {
                                info->invalid_sudoku = true;
                        } else {
                                Table_put(info->table, key, entry_p);
                        }   
                }
        }
}

void validate_submaps(UArray2_T map, void *info_p) {
        struct sudokuInfo *info = info_p;
        for (int c = 0; c < 9; c += 3) {
                for (int r = 0; r < 9; r += 3) {
                        info->table = Table_new(9, NULL, NULL);
                        validate_one_submap(c, r, map, info);
                        Table_free(&(info->table));
                }
        }
}