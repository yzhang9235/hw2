#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <table.h>
#include <atom.h>
#include <ctype.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "bit2.h"
#include "unblackedges.h"
#include <seq.h>

struct pixel {
    int col;
    int row;
    int bit;
    bool visited;
};

int main (int argc, char *argv[]) 
{
        FILE *input;

        /* process command-line arguments */
        if (argc > 2) {
                fprintf(stderr, "too many arguments\n");
                exit(EXIT_FAILURE);
        } else if (argc == 1) {
                /* read from standard input */
                input = stdin;
        } else if (argc == 2) {
                /* open the given file */
                input = fopen(argv[1], "rb"); 
                /* CRE: file must be accessible */ 
                if (input == NULL) {
                        fprintf(stderr, "cannot open file '\n");
                        exit(EXIT_FAILURE);
                }
        }

        Pnmrdr_T reader = Pnmrdr_new(input);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);
        if (data.type != Pnmrdr_bit) {
                fprintf(stderr, "image is not a bitmap\n");
                exit(EXIT_FAILURE);
        } else if (data.width == 0 || data.height == 0) {
                fprintf(stderr, "invalid width and height\n");
                exit(EXIT_FAILURE);
        }
        Bit2_T bitmap = Bit2_new(data.width, data.height);
        Bit2_T blackedges = Bit2_new(data.width, data.height);
        Bit2_map_row_major(bitmap, populate_data, &reader);
        track_blackedges(bitmap, blackedges);
        Bit2_map_row_major(bitmap, unblackedges, &blackedges);
        
        // Bit2_map_row_major(bitmap, populate_data, &reader);
        // Bit2_map_row_major(bitmap, unblackedges, &blackedges);
        // Bit2_map_col_major(bitmap, unblackedges, &blackedges);

        printf("P1\n %d %d\n", data.width, data.height);
        Bit2_map_row_major(bitmap, print_image, NULL);

        Bit2_free(&bitmap);
        Bit2_free(&blackedges);
}

void populate_data(int col, int row, Bit2_T bit2, int bit, void *cl) 
{
        (void) bit;
        Pnmrdr_T *reader_p = cl;
        Bit2_put(bit2, col, row, Pnmrdr_get(*reader_p));
}

// void unblackedges(int col, int row, Bit2_T bitmap, int bit, void *cl)
// {
//         (void) bit;
//         Bit2_T *blackedges_p = cl;
//         Bit2_T blackedges = *blackedges_p;
//         if (is_blackedge(col, row, bitmap, blackedges) == true) {
//                 Bit2_put(bitmap, col, row, 0);
//                 Bit2_put(*blackedges_p, col, row, 1);
//         }    
// }

// bool is_blackedge(int col, int row, Bit2_T bitmap, Bit2_T blackedges) {
//         int bit = Bit2_get(bitmap, col, row);
//         int height = Bit2_height(bitmap);
//         int width = Bit2_width(bitmap);
//         int left, right, top, bottom = -1;
//         bool is_edge = col == 0 || col == width - 1 || row == 0 || row == height - 1;
//         if (is_edge == false) {
//                 left = Bit2_get(blackedges, col - 1, row);
//                 right = Bit2_get(blackedges, col + 1, row);
//                 top = Bit2_get(blackedges, col, row - 1);
//                 bottom = Bit2_get(blackedges, col - 1, row);
//         }
        
//         if (bit == 1) {
//                 if (is_edge == true) {
//                         return true;
//                 } else if (left == 1 or right == 1 or top == 1 or bottom == 1) {
//                         return true;
//                 } else {
//                         return false;
//                 }
//         } else {
//                 return false;
//         }
// }
void track_blackedges(Bit2_T bitmap, Bit2_T blackedges) {
        Seq_T pq = Seq_new(0);
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        for (int c = 0; c < width; c++) {
                if (Bit2_get(bitmap, c, 0) == 1 || Bit2_get(bitmap, c, height - 1) == 1) {
                        struct pixel blackedge;
                        blackedge.col = c;
                        blackedge.row = 0;
                        blackedge.bit = 1;
                        blackedge.visited = true;
                        Seq_addhi(pq, &blackedge);
                        Bit2_put(blackedges, c, 0, 1);
                }     
        }

        for (int r = 0; r < height; r++) {
                if (Bit2_get(bitmap, 0, r) == 1 || Bit2_get(bitmap, width - 1, r) == 1) {
                        struct pixel blackedge;
                        blackedge.col = 0;
                        blackedge.row = r;
                        blackedge.bit = 1;
                        blackedge.visited = true;
                        Seq_addhi(pq, &blackedge);
                        Bit2_put(blackedges, 0, r, 1);
                }     
        }
        while (Seq_length(pq) > 0) {
                struct pixel *next = Seq_remlo(pq);
                if (next->col - 1 > 0) {
                        if (Bit2_get(bitmap, next->col - 1, next->row) == 1 and next->visited == false) {
                            struct pixel left;
                            left.col = next->col - 1;
                            left.row = next->row;
                            left.bit = 1;
                            Seq_addhi(pq, &left);
                            left.visited = true;
                            Bit2_put(blackedges, left.col, left.row, 1);
                    } 
                }
                if (next->col + 1 < width) {
                        if (Bit2_get(bitmap, next->col + 1, next->row) == 1 and next->visited == false) {
                        struct pixel right;
                        right.col = next->col + 1;
                        right.row = next->row;
                        right.bit = 1;
                        Seq_addhi(pq, &right);
                        right.visited = true;
                        Bit2_put(blackedges, right.col, right.row, 1);
                }
                }
                if (next->row - 1 > 0) {
                        if (Bit2_get(bitmap, next->col, next->row - 1) == 1 and next->visited == false) {
                            struct pixel top;
                            top.col = next->col;
                            top.row = next->row - 1;
                            top.bit = 1;
                            Seq_addhi(pq, &top);
                            top.visited = true;
                            Bit2_put(blackedges, top.col, top.row, 1);
                    }
                }
                if (next->row + 1 < height) {
                        if (Bit2_get(bitmap, next->col, next->row + 1) == 1 and next->visited == false) {
                            struct pixel bottom;
                            bottom.col = next->col;
                            bottom.row = next->row + 1;
                            bottom.bit = 1;
                            Seq_addhi(pq, &bottom);
                            bottom.visited = true;
                            Bit2_put(blackedges, bottom.col, bottom.row, 1);
                    }
                }
        }  
}

void unblackedges(int col, int row, Bit2_T bit2, int bit, void *cl) 
{
        (void)bit;
        Bit2_T *blackedges_p = cl;
        Bit2_T blackedges = *blackedges_p;
        if (Bit2_get(blackedges, col, row) == 1) {
                Bit2_put(bit2, col, row, 0);
        }
}

void print_image(int col, int row, Bit2_T bitmap, int bit, void *cl) 
{
        (void) bit;
        (void) cl;
        int nextbit = Bit2_get(bitmap, col, row);
        printf("%d", nextbit);
}