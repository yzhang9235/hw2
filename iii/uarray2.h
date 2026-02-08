/**************************************************************
 *
 *                     uarray2.h
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     This header contains the interface for the UArray2 data structure, which
 *     is a 2D version of Hanson's unboxed array (UArray) data structure. The
 *     interface contains functions to allocate and initialize the array, get
 *     height, width, and size information, add and access elements, mapping,
 *     and deallocating the array.
 *
 **************************************************************/

#ifndef UARRAY2_H
#define UARRAY2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray.h"
 
#define T UArray2_T
typedef struct T *T;

T UArray2_new(int width, int height, int size);
void *UArray2_at(T uarray2, int col, int row);
int UArray2_width(T uarray2);
int UArray2_height(T uarray2);
int UArray2_size(T uarray2);
void UArray2_map_col_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl);
void UArray2_map_row_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl);
void UArray2_free(T *uarray2);

#undef T
#endif