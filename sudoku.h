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

#endif