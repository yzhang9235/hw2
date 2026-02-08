/**************************************************************
 *
 *                     sudoku.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *             This file provides the implementation of Sudoku validation.
 *             It reads a PGM image using Pnmrdr, stores the pixels in UArray2
 *             and verifies that whether it is a solved Sudoku puzzle.
 *
 **************************************************************/
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

/********** main ********
 *
 * Driver program for validating a 9*9 Sudoku puzzle
 * stored in a PGM image.
 *
 * Arguments:
 *      argc: number of command-line arguments
 *      argv: argument vector
 *            argv[1] (optional) is the input file name
 *
 * Return: 0 on successful validation; exit with failure if the Sudoku 
 *         is invalid
 *
 * Expects:
 *      At most one command-line argument
 *      Input file is accessible and in valid PGM graymap format
 *
 * Notes:
 *      If no file name is provided, input is read from stdin
 *      Will CRE if too many arguments are provided
 *      Will CRE if input file cannot be opened or has invalid dimentions
 *      or denominator
 *      Dynamically allocated memory is freed before program exit
 ************************/
int main (int argc, char *argv[]) 
{
        FILE *input;
        // process command-line arguments
        if (argc > 2) {
                fprintf(stderr, "too many arguments\n");
                exit(EXIT_FAILURE);
        } else if (argc == 1) {
                // read from standard input
                input = stdin;
        } else if (argc == 2) {
                // open the given file
                input = fopen(argv[1], "rb"); 
                // CRE: file must be accessible 
                if (input == NULL) {
                        fprintf(stderr, "cannot open file '\n");
                        exit(EXIT_FAILURE);
                }
        }
        //check for invalid input
        Pnmrdr_T reader = Pnmrdr_new(input);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);
        if (data.type not_eq Pnmrdr_gray) {
                fprintf(stderr, "image is not a graymap\n");
                exit(EXIT_FAILURE);
        } else if (data.denominator not_eq 9) {
                fprintf(stderr, "invalid denominator\n");
                exit(EXIT_FAILURE);
        } else if (data.width not_eq 9 or data.height not_eq 9) {
                fprintf(stderr, "invalid width and height\n");
                exit(EXIT_FAILURE);
        }
        // initialize a uarray2
        UArray2_T data_arr = UArray2_new(data.width, data.height, sizeof(int));
        // populate uarray2 with the data from the file 
        UArray2_map_row_major(data_arr, populate_data, &reader);

        // Check that no two pixels have the same intensity in each row, column,
        // or submap
        struct sudokuInfo info;
        info.table = Table_new(9, NULL, NULL);
        info.invalid_sudoku = false;
        UArray2_map_row_major(data_arr, validate_row, &info);
        UArray2_map_col_major(data_arr, validate_col, &info);
        validate_submaps(data_arr, &info);

        // free data structures
        Table_free(&(info.table));
        UArray2_free(&data_arr);
        Pnmrdr_free(&reader);
        fclose(input);
        
        if (info.invalid_sudoku == true) {
                exit(EXIT_FAILURE);
        }
        return EXIT_SUCCESS;
}

/********** populate_data ********
 *
 *  Get next pixel from input file and stored in the UArray2 at current position
 *
 * Arguments:
 *      int col: column index in the UArray2 (unused)
 *      int row: row index in the UArray2 (unused)
 *      UArray2_T uarray2: the 2D array to store pixel values (unused)
 *      void *p1: pointer to the current element in the UArray2
 *      void *p2: pointer to the Pnmrdr_T reader used to get the next pixel
 *
 * Return: nothing
 *
 * Expects:
 *      P1 and P2 are not NULL
 *
 * Notes: nothing
 *
 ************************/
void populate_data(int col, int row, UArray2_T uarray2, void *p1, void *p2) 
{
        (void) col, (void) row, (void) uarray2;
        int *entry_p = p1;
        Pnmrdr_T *reader_p = p2;
        // read next pixel into uarray2
        *entry_p = Pnmrdr_get(*reader_p); 
}

/********** is_correct_intensity ********
 *
 *  Check if a pixel has a valid intensity
 *
 * Arguments:
 *      int: given pixel value
 *
 * Return: 
 *      true if pixel is not zero and not greater than 9, false otherwise
 *
 * Expects: nothing
 *
 * Notes:  nothing
 *
 ************************/
bool is_correct_intensity(int pixel)
{
        return pixel > 0 and pixel < 10;
}

/********** validate_row ********
 *
 *  Checks for duplicate pixel intensities in a single Sudoku row.
 *
 * Arguments:
 *      int col: current column index in the row
 *      int row: current row index (unused)
 *      UArray2_T uarray2: the given uarray2 (unused)
 *      void *p1: pointer to the current pixel value
 *      void *info_p: pointer to sudokuInfo struct containing the 
 *                    table and a boolean 
 *
 * Return: nothing
 *
 * Expects:
 *      p1 and info_p are not NULL
 *      The sudokuInfo table is valid
 *
 * Notes:
 *      Resets the table at the beginning of each row
 *      Calls duplicate_check to mark invalid if duplicates exist
 *      Will update info_p->invalid_sudoku to true if any duplicate found
 *
 ************************/
void validate_row(int col, int row, UArray2_T uarray2, void *p1, void *info_p)
{
        (void) row, (void) uarray2;
        struct sudokuInfo *info = info_p;
        // reset table at beginning of each row
        if (col == 0) {
                Table_free(&(info->table));
                info->table = Table_new(9, NULL, NULL);
        }
        duplicate_check(p1, info);
}

/********** validate_col ********
 *
 *  Checks for duplicate pixel intensities in a single Sudoku column.
 *
 * Arguments:
 *      int col: current column index in the row
 *      int row: current row index (unused)
 *      UArray2_T uarray2: the given uarray2 (unused)
 *      void *p1: pointer to the current pixel value
 *      void *info_p: pointer to sudokuInfo struct containing the 
 *                    table and a boolean 
 *
 * Return: nothing
 *
 * Expects:
 *      p1 and info_p are not NULL
 *      The sudokuInfo table is valid
 *
 * Notes:
 *      Resets the table at the beginning of each column
 *      Calls duplicate_check to mark invalid if duplicates exist
 *      Will update info_p->invalid_sudoku to true if any duplicate found
 *
 ************************/
void validate_col(int col, int row, UArray2_T uarray2, void *p1, void *info_p)
{
        (void) col;
        (void) uarray2;
        struct sudokuInfo *info = info_p;
        // reset table at beginning of each column
        if (row == 0) {
                Table_free(&(info->table));
                info->table = Table_new(9, NULL, NULL);
        }
        duplicate_check(p1, info);
}

/********** validate_submaps ********
 *
 *  Validates all 3x3 submaps of a 9x9 Sudoku puzzle.
 *  Resets the table before checking each submap and
 *  calls validate_one_submap for actual duplicate checking.
 *
 * Arguments:
 *      UArray2_T map: given UArray2 containing Sudoku pixel values
 *      void *info_p: pointer to sudokuInfo struct storing table and 
 *                    boolean for validation
 *
 * Return: nothing
 *
 * Expects:
 *      map and info_p are not NULL
 *
 * Notes:
 *      Will update info_p->invalid_sudoku to true if any duplicate found
 *      Table is reset for each submap
 *
 ************************/
void validate_submaps(UArray2_T map, void *info_p) 
{
        struct sudokuInfo *info = info_p;
        for (int c = 0; c < 9; c += 3) {
                for (int r = 0; r < 9; r += 3) {
                        // reset table before checking new submap
                        Table_free(&(info->table));
                        info->table = Table_new(9, NULL, NULL);
                        validate_one_submap(c, r, map, info);
                }
        }
}

/********** validate_one_submap ********
 *
 *  Checks a single 3x3 submap of a Sudoku grid for duplicate pixel values.
 *
 * Arguments:
 *      int start_c: starting column index of the submap
 *      int start_r: starting row index of the submap
 *      UArray2_T map: given UArray2 containing Sudoku pixel values
 *      void *info_p: pointer to sudokuInfo struct storing table and 
 *                    boolean for validation
 *
 * Return: nothing
 *
 * Expects:
 *      start_c and start_r are multiples of 3 within [0,6]
 *      map and info_p are not NULL
 *
 * Notes:
 *      Calls duplicate_check on each element in the submap
 *      Updates info_p->invalid_sudoku if duplicates are found
 ************************/
void validate_one_submap(int start_c, int start_r, UArray2_T map, void *info_p)
{
        struct sudokuInfo *info = info_p;
        // check 3x3 submap starting at index start_c, start_r
        for (int c = start_c; c < start_c + 3; c++) {
                for (int r = start_r; r < start_r + 3; r++) {
                        int *entry_p = UArray2_at(map, c, r);
                        duplicate_check(entry_p, info);  
                }
        }
}

/********** duplicate_check ********
 *
 *  Checks whether a given pixel value has already appeared in the current row,
 *  column, or submap of a Sudoku grid. Updates the invalid_sudoku boolean if
 *  duplicate or invalid intensity is found.
 *
 * Arguments:
 *      void *entry: pointer to the current pixel value
 *      void *info_p: pointer to sudokuInfo struct storing helper table and 
 *                    validation boolean
 *
 * Return: nothing
 *
 * Expects:
 *      entry and info_p are not NULL
 *
 * Notes:
 *      Valid values are inserted into the table to track occurrence.
 ************************/
void duplicate_check(void *entry, void *info_p) 
{
        struct sudokuInfo *info = info_p;
        int *entry_p = entry;
        //change the type to atom_int in order to put in table as a key
        const void *key = Atom_int(*entry_p);
        if (Table_get(info->table, key) not_eq NULL) { // found repeat
                info->invalid_sudoku = true;
        } else {
                // valid intensity case
                if (is_correct_intensity(*entry_p) == true) { 
                        Table_put(info->table, key, entry_p);
                } else { // invalid intensity case
                        info->invalid_sudoku = true;
                        Table_put(info->table, key, entry_p);
                }
                
        }
}