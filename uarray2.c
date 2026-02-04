/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     
 *
 **************************************************************/

#include "uarray2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray.h"
#include <assert.h>
#include <iso646.h>

#define T UArray2_T

struct T {
        int width;
        int height;
        int size;
        UArray_T data;
};

T UArray2_new(int width, int height, int size) 
{
        assert(width >= 0 && height >= 0 && size >= 0);
        T curr_array = malloc(sizeof(*curr_array));
        assert(curr_array != NULL); // check malloc worked
        curr_array->data = UArray_new(height * width, size);
        curr_array->width = width;
        curr_array->height = height;
        curr_array->size = size; 
        return curr_array;
}

void *UArray2_at(T uarray2, int col, int row) 
{
        assert(uarray2 != NULL);
        assert(col < uarray2->width && row < uarray2->height);
        int index = (uarray2->width * row) + col;
        return UArray_at(uarray2->data, index);
}

int UArray2_width(T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

int UArray2_height(T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

void UArray2_map_col_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl) 
{
        assert(uarray2 != NULL);
        for (int c = 0; c < uarray2->width; c++) {
                for (int r = 0; r < uarray2->height; r++) {
                        void *p1 = UArray2_at(uarray2, c, r);
                        apply(c, r, uarray2, p1, cl);
                }
        }
}

void UArray2_map_row_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl) 
{
        assert(uarray2 != NULL);
        for (int r = 0; r < uarray2->height; r++) {
                for (int c = 0; c < uarray2->width; c++) {
                        void *p1 = UArray2_at(uarray2, c, r);
                        apply(c, r, uarray2, p1, cl);
                }
        }
}

void UArray2_free(T *uarray2) 
{
        assert(*uarray2 != NULL && uarray2 != NULL);
        UArray_free(&((*uarray2)->data));
        free(*uarray2);
        *uarray2 = NULL;
}

#undef T