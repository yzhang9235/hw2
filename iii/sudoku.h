/**************************************************************
 *
 *                     sudoku.h
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary: 
 *             This header provides an interface for validating a Sudoku stored
 *             in a 2D UArray2. The interface contains functions to populate 
 *             data and verify that rows, columns and 3*3 subarrays satisfy 
 *             Sudoku rules.
 *     
 *
 **************************************************************/
#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray.h"
#include "uarray2.h"

void populate_data(int col, int row, UArray2_T uarray2, void *p1, void *p2);
bool is_correct_intensity(int pixel);
void validate_row(int col, int row, UArray2_T uarray2, void *p1, void *info_p);
void validate_col(int col, int row, UArray2_T uarray2, void *p1, void *info_p);
void validate_submaps(UArray2_T map, void *info_p);
void validate_one_submap(int start_c, int start_r, UArray2_T map, void *info_p);
void duplicate_check(void *entry, void *info_p);

#endif