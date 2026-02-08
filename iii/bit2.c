/**************************************************************
 *
 *                     bit2.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary: 
 *     This file provides the implementation of the Bit2 interface. It defines
 *     Bit2 as a struct containing the width and height of the array and a one-
 *     dimensional array of bits stored in row major order. Bit2 supports 
 *     creation, access and modification of bits unsing column and row indices. 
 *     It also provides row and column major mapping functions that accept the 
 *     applied function for each bit in the bitmap.
 *   
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iso646.h>
#include "bit2.h"
#include <assert.h>

#define T Bit2_T

struct T {
        int width;
        int height;
        Bit_T data;
};

/********** Bit2_new ********
 *
 * Description: allocates, initializes, and returns a new Bit2 instance with 
 *              the specified width, height, and element size
 *
 * Parameters: 
 *          int width: the width of the 2D array
 *          int height : the height of the 2D array
 *      
 * Return: The new bitmap
 *      
 * Expects: width >= 0, height >= 0
 *
 * Notes: CRE if width or height is negative  or if memory allocation fails
 *      
 ************************/
T Bit2_new(int width, int height) 
{
        assert(width >= 0 and height >= 0);
        T curr_bit = malloc(sizeof(*curr_bit));
        curr_bit->data = Bit_new(height * width);
        curr_bit->width = width;
        curr_bit->height = height;
        return curr_bit;
}

/********** Bit2_get ********
 *
 * Description: Get the bit at given position of bitmap
 *
 * Parameters: 
 *          T bit2: given bitmap
 *          int col: the column index of the position
 *          int row: the row index of the position
 *      
 * Return: the bit at index [col, row]
 *      
 * Expects: Bit2 is not NULL, row and col are between 0 and height and width,
 *          respectively
 *      
 * Notes: CRE when parameters are NULL
 *      
 ************************/
int Bit2_get(T bit2, int col, int row) 
{
        assert(bit2 not_eq NULL);
        int index = (bit2->width * row) + col;
        return Bit_get(bit2->data, index);
}

/********** Bit2_put ********
 *
 * Description: Put gieven bit at given position of bitmap
 *
 * Parameters: 
 *          T bit2: given bitmap
 *          int col: the column index of the position
 *          int row: the row index of the position
 *          int bit: the given bit to be stored at the given position
 *      
 * Return: the previous value of the bit at given position
 *      
 * Expects: Bit2 is not NULL, row and col are between 0 and height and width,
 *          respectively
 *      
 * Notes: CRE when parameters are NULL
 *      
 ************************/
int Bit2_put(T bit2, int col, int row, int bit) 
{
        assert(bit2 not_eq NULL);
        assert(col < bit2->width and row < bit2->height);
        int index = (bit2->width * row) + col;
        return Bit_put(bit2->data, index, bit);
}

/********** Bit2_width ********
 *
 * Description: get the width of the bitmap
 *
 * Parameters: 
 *          T bit2: given bitmap
 *      
 * Return: the width of the given bitmap
 *      
 * Expects: bit2 is not NULL
 *      
 * Notes: CRE if bit2 is NULL
 *      
 ************************/
int Bit2_width(T bit2) 
{
        assert(bit2 not_eq NULL);
        return bit2->width;
}

/********** B2_height ********
 *
 * Description: get the height of the bitmap
 *
 * Parameters: 
 *          T bit2: given bitmap
 *      
 * Return: the height of the given bitmap
 *      
 * Expects: bit2 is not NULL
 *      
 * Notes: CRE if bit2 is NULL
 *      
 ************************/
int Bit2_height(T bit2)
{
        assert(bit2 not_eq NULL);
        return bit2->height;
}

/********** Bit2_map_col_major ********
 *
 * Description: calls the given apply() function on each element in the Bit2 in
 *              column major order 
 *
 * Parameters: 
 *          T bit2: given Bit2
 *          void apply(int row, int col, T bit2, void *p1):
 *                    the apply function on each element 
 *                    -- int row: the row of the current bit
 *                    -- int col: the column of the current bit
 *                    -- T bit2: the given Bit2
 *                    -- void *p1: pointer to bit at [col, row]
 *          void *cl: pointer to client supplied closure
 *      
 * Return: none
 *      
 * Expects: bit2 is not NULL and apply function is not NULL
 *      
 * Notes: none
 *      
 ************************/
void Bit2_map_col_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl)
{
        assert(bit2 not_eq NULL);
        for (int c = 0; c < bit2->width; c++) {
                for (int r = 0; r < bit2->height; r++) {
                        int bit = Bit2_get(bit2, c, r);
                        apply(c, r, bit2, bit, cl);
                }
        }
}

/********** Bit2_map_row_major ********
 *
 * Description: calls the given apply() function on each element in the Bit2 in
 *              row major order 
 *
 * Parameters: 
 *          T bit2: given Bit2
 *          void apply(int row, int col, T bit2, void *p1):
 *                    the apply function on each element 
 *                    -- int row: the row of the current bit
 *                    -- int col: the column of the current bit
 *                    -- T bit2: the given Bit2
 *                    -- void *p1: pointer to bit at [col, row]
 *          void *cl: pointer to client supplied closure
 *      
 * Return: none
 *      
 * Expects: bit2 is not NULL and apply function is not NULL
 *      
 * Notes: none
 *      
 ************************/
void Bit2_map_row_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl)
{
        assert(bit2 not_eq NULL);
        for (int r = 0; r < bit2->height; r++) {
                for (int c = 0; c < bit2->width; c++) {
                        int bit = Bit2_get(bit2, c, r);
                        apply(c, r, bit2, bit, cl);
                }
        }
}    

/********** Bit2_free ********
 *
 * Description: deallocates and clears the given Bit2 instance
 *
 * Parameters: 
 *          T *bit2: pointer to given Bit2
 *      
 * Return: none
 *      
 * Expects: bit2 and *bit2 are not NULL
 *      
 * Notes: after return, *bit2 is NULL
 *      
 ************************/
void Bit2_free(T *bit2)
{
        assert(*bit2 not_eq NULL and bit2 not_eq NULL);
        Bit_free(&((*bit2)->data));
        free(*bit2);
}


#undef T