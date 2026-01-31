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

#define T UArray2_T
typedef struct T *T;

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
 * Expects: parameters should be non-negative
 *      
 * Notes: CRE when parameters are NULL or negative or when there is a failure to
 *        allocate memory
 *      
 ************************/
T UArray2_new(int width, int height, int size);

/********** Uarray2_at ********
 *
 * Description: Get the element at given position of the 2D array
 *
 * Parameters: 
 *          T uarray2: given array
 *          int row: the row index of the position
 *          int col: the column index of the position
 *      
 * Return: a pointer to the element at index [row, col]
 *      
 * Expects: uarray2 is not NULL, row and col are between 0 and width and height,
 *          respectively
 *      
 * Notes: CRE when parameters are NULL or out of bounds
 *      
 ************************/
void *UArray2_at(T uarray2, int row, int col);

/********** Uarray2_width ********
 *
 * Description: get the width of the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the width of the given array
 *      
 * Expects: none
 *      
 * Notes: none
 *      
 ************************/
int UArray2_width(T uarray2);

/********** Uarray2_height ********
 *
 * Description: get the height of the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the height of the given array
 *      
 * Expects: none
 *      
 * Notes: none
 *      
 ************************/
int UArray2_height(T uarray2);

/********** Uarray2_size ********
 *
 * Description: get the size of the elements stored in the array
 *
 * Parameters: 
 *          T uarray2: given array
 *      
 * Return: the size of the elements stored in the given array
 *      
 * Expects: none
 *      
 * Notes: none
 *      
 ************************/
int UArray2_size(T uarray2);

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
 *                    -- void *p1: pointer to element at [row, col]
 *                    -- void *p2: pointer to client supplied closure
 *          void *cl: client supplied closure
 *      
 * Return: none
 *      
 * Expects: the parameter is not NULL
 *      
 * Notes: none
 *      
 ************************/
void UArray2_map_col_major(T uarray2, 
                           void apply(int row, int col, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl);

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
 *                    -- void *p1: pointer to element at [row, col]
 *                    -- void *p2: pointer to client supplied closure
 *          void *cl: client supplied closure
 *      
 * Return: none
 *      
 * Expects: the parameter is not an empty array
 *      
 * Notes: none
 *      
 ************************/
void UArray2_map_row_major(T uarray2, 
                           void apply(int row, int col, T uarray2, 
                                      void *p1, void *p2), 
                           void *cl);

/********** Uarray2_free ********
 *
 * Description: deallocates and clears the given array
 *
 * Parameters: 
 *          T *uarray2: pointer to given array
 *      
 * Return: none
 *      
 * Expects: uarray2 is not NULL
 *      
 * Notes: CRE if uarray2 is NULL
 *      
 ************************/
void UArray2_free(T *uarray2);

#undef T
#endif