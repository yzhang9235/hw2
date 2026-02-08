/**************************************************************
 *
 *                     bit2.h
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     This header provides an interface that supports two-dimensional
 *     arrays of bits. The interface contains functions to allocate 
 *     and initialize the array, get height, width, and size information,
 *     add and access elements, mapping, and deallocating the array.
 *
 **************************************************************/

#ifndef BIT2_H
#define BIT2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bit.h"

#define T Bit2_T
typedef struct T *T;

T Bit2_new(int width, int height);
int Bit2_get(T bit2, int col, int row);
int Bit2_put(T bit2, int col, int row, int bit); 
int Bit2_width(T bit2);
int Bit2_height(T bit2);
void Bit2_map_col_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl);
void Bit2_map_row_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl);
void Bit2_free(T *bit2);

#undef T
#endif