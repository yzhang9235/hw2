/**************************************************************
 *
 *                     bit2.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *   
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

T Bit2_new(int width, int height) 
{
        assert(width >= 0 && height >= 0);
        T curr_bit = malloc(height * width);
        curr_bit->data = Bit_new(height * width);
        curr_bit->width = width;
        curr_bit->height = height;
        return curr_bit;
}

int Bit2_get(T bit2, int col, int row) 
{
        assert(bit2 != NULL);
        int index = (bit2->width * row) + col;
        return Bit_get(bit2->data, index);
}

int Bit2_put(T bit2, int col, int row, int bit) 
{
        assert(bit2 != NULL);
        assert(col < bit2->width && row < bit2->height);
        int index = (bit2->width * row) + col;
        return Bit_put(bit2->data, index, bit);
}

int Bit2_width(T bit2) 
{
        return bit2->width;
}

int Bit2_height(T bit2)
{
        return bit2->height;
}

void Bit2_map_col_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl)
{
        assert(bit2 != NULL);
        for (int c = 0; c < bit2->width; c++) {
                for (int r = 0; r < bit2->height; r++) {
                        int bit = Bit2_get(bit2, c, r);
                        apply(c, r, bit2, bit, cl);
                }
        }
}

void Bit2_map_row_major(T bit2, 
                        void apply(int col, int row, T bit2, int bit, void *p1),
                        void *cl)
{
        assert(bit2 != NULL);
        for (int r = 0; r < bit2->height; r++) {
                for (int c = 0; c < bit2->width; c++) {
                        int bit = Bit2_get(bit2, c, r);
                        apply(c, r, bit2, bit, cl);
                }
        }
}    
                   
void Bit2_free(T *bit2)
{
        assert(*bit2 != NULL && bit2 != NULL);
        free((*bit2)->data);
        free(*bit2);
}


#undef T


