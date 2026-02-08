/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     This file provides the implementation of the UArray2 interface, which
 *     defines a UArray2 as a struct containing the width, height, size, and a
 *     one-dimensional UArray containing all of the data in the UArray2 stored
 *     in row major order. This file also implements the functions to allocate 
 *     and initialize the array, get height, width, and size information, add 
 *     and access elements, mapping, and deallocating the array.
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

/********** Uarray2_new ********
 *
 * Description: allocates, initializes, and returns a new UArray2 instance with 
 *              the specified width, height, and element size
 *
 * Parameters: 
 *          int width: the width of the 2D array
 *          int height : the height of the 2D array
 *          int size : size of elements
 *      
 * Return: The new array
 *      
 * Expects: width >= 0, height >= 0, size > 0
 *
 * Notes: CRE if width or height is negative, if size <= 0,
 *        or if memory allocation fails
 *      
 ************************/
T UArray2_new(int width, int height, int size) 
{
        assert(width >= 0 and height >= 0 and size >= 0);
        T curr_array = malloc(sizeof(*curr_array));
        assert(curr_array not_eq NULL); // check malloc worked
        curr_array->data = UArray_new(height * width, size);
        curr_array->width = width;
        curr_array->height = height;
        curr_array->size = size; 
        return curr_array;
}

/********** Uarray2_at ********
 *
 * Description: Get the element at given position of the 2D array
 *
 * Parameters: 
 *          T uarray2: given array
 *          int row: the row index of the position
 *          int col: the column index of the position
 *      
 * Return: a pointer to the element at index [col, row]
 *      
 * Expects: uarray2 is not NULL, row and col are between 0 and height and width,
 *          respectively
 *      
 * Notes: CRE when parameters are NULL or indices are out of bounds
 *      
 ************************/
void *UArray2_at(T uarray2, int col, int row) 
{
        assert(uarray2 not_eq NULL);
        assert(col < uarray2->width and row < uarray2->height);
        int index = (uarray2->width * row) + col;
        return UArray_at(uarray2->data, index);
}
/********** Uarray2_width ********
 *
 * Description: get the width of the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the width of the given array
 *      
 * Expects: uarray2 is not NULL
 *      
 * Notes: CRE if uarray2 is NULL
 *      
 ************************/
int UArray2_width(T uarray2) 
{
        assert(uarray2 not_eq NULL);
        return uarray2->width;
}

/********** Uarray2_height ********
 *
 * Description: get the height of the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the height of the given array
 *      
 * Expects: uarray2 is not NULL
 *      
 * Notes: CRE if uarray2 is NULL
 *      
 ************************/
int UArray2_height(T uarray2) 
{
        assert(uarray2 not_eq NULL);
        return uarray2->height;
}

/********** Uarray2_size ********
 *
 * Description: get the size of the elements stored in the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the size of the elements stored in the given array
 *      
 * Expects: uarray2 is not NULL
 *      
 * Notes: CRE if uarray2 is NULL
 *      
 ************************/
int UArray2_size(T uarray2)
{
        assert(uarray2 not_eq NULL);
        return uarray2->size;
}

/********** Uarray2_map_col_major ********
 *
 * Description: calls the given apply() function on each element in the array in
 *              column major order 
 *
 * Parameters: 
 *          T uarray2: given array
 *          void apply(int row, int col, T uarray2, void *p1, void *p2):
 *                    the apply function on each element 
 *                    -- int row: the row of the current element
 *                    -- int col: the column of the current element
 *                    -- T uarray2: the given array
 *                    -- void *p1: pointer to element at [col, row]
 *                    -- void *p2: pointer to client supplied closure
 *          void *cl: client supplied closure
 *      
 * Return: none
 *      
 * Expects: uarray2 is not NULL and apply function is not NULL
 *      
 * Notes: none
 *      
 ************************/
void UArray2_map_col_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl) 
{
        assert(uarray2 not_eq NULL);
        for (int c = 0; c < uarray2->width; c++) {
                for (int r = 0; r < uarray2->height; r++) {
                        void *p1 = UArray2_at(uarray2, c, r);
                        apply(c, r, uarray2, p1, cl);
                }
        }
}

/********** Uarray2_map_row_major ********
 *
 * Description: calls the given apply() function on each element in the array in
 *              row major order 
 *
 * Parameters: 
 *          T uarray2: given array
 *          void apply(int row, int col, T uarray2, void *p1, void *p2):
 *                    the apply function on each element 
 *                    -- int row: the row of the current element
 *                    -- int col: the column of the current element
 *                    -- T uarray2: the given array
 *                    -- void *p1: pointer to element at [col, row]
 *                    -- void *p2: pointer to client supplied closure
 *          void *cl: client supplied closure
 *      
 * Return: none
 *      
 * Expects: uarray2 is not NULL and apply function is not NULL
 *      
 * Notes: none
 *      
 ************************/
void UArray2_map_row_major(T uarray2, 
                           void apply(int col, int row, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl) 
{
        assert(uarray2 not_eq NULL);
        for (int r = 0; r < uarray2->height; r++) {
                for (int c = 0; c < uarray2->width; c++) {
                        void *p1 = UArray2_at(uarray2, c, r);
                        apply(c, r, uarray2, p1, cl);
                }
        }
}

/********** Uarray2_free ********
 *
 * Description: deallocates and clears the given array
 *
 * Parameters: 
 *          T *uarray2: pointer to given array
 *      
 * Return: none
 *      
 * Expects: uarray2 and *uarray2 are not NULL
 *      
 * Notes: after return, *uarray2 is NULL
 *      
 ************************/
void UArray2_free(T *uarray2) 
{
        assert(*uarray2 not_eq NULL and uarray2 not_eq NULL);
        UArray_free(&((*uarray2)->data));
        free(*uarray2);
        *uarray2 = NULL;
}

#undef T