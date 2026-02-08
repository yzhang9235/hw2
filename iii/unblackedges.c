/**************************************************************
 *
 *                     unblackedges.c
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     This file implements the functions defined in the unblackedges interface.
 *     The program reads data from a provided portable bitmap into a Bit2
 *     instance, identifies and removes black edges, and outputs a plain
 *     portable bitmap with black edges removed to standard output.
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <pnmrdr.h>
#include "bit2.h"
#include "unblackedges.h"
#include <seq.h>

// contains column and row index for a pixel to be added to a priority queue
struct Pixel {
    int col;
    int row;
};

/********** main ********
 *
 * Description: Driver program for removing black edges from a portable bitmap
 *              file
 *
 * Parameters: 
 *          int argc: number of command-line arguments
 *          char *argv[]: argument vector
 *                        argv[1] (optional) is the input file name
 *      
 * Return: EXIT_SUCCESS unless program was exited eariler due to CREs.
 *      
 * Expects: At most one command-line argument, Input file is accessible and in 
 *          valid PBM bitmap format
 *
 * Notes: If no file name is provided, input is read from stdin
 *        CRE if too many arguments are provided
 *        CRE if input file cannot be opened or has invalid dimensions or
 *        denominator
 *        Dynamically allocated memory is freed before program exit
 *      
 ************************/
int main (int argc, char *argv[]) 
{
        FILE *input;
        // process command-line arguments
        if (argc > 2) {
                fprintf(stderr, "too many arguments\n");
                exit(EXIT_FAILURE);
        } else if (argc == 1) {
                // read from standard input
                input = stdin;
        } else if (argc == 2) {
                // open the given file
                input = fopen(argv[1], "rb"); 
                // CRE: file must be accessible 
                if (input == NULL) {
                        fprintf(stderr, "cannot open file '\n");
                        exit(EXIT_FAILURE);
                }
        }

        Pnmrdr_T reader = Pnmrdr_new(input);
        Pnmrdr_mapdata data = Pnmrdr_data(reader);
        // CRE: input is not a bitmap
        if (data.type not_eq Pnmrdr_bit) {
                fprintf(stderr, "image is not a bitmap\n");
                exit(EXIT_FAILURE);
        } else if (data.width == 0 or data.height == 0) {
                fprintf(stderr, "invalid width and height\n");
                exit(EXIT_FAILURE);
        }
        Bit2_T bitmap = Bit2_new(data.width, data.height);
        Bit2_T blackedges = Bit2_new(data.width, data.height);
        // populate data to bitmap
        Bit2_map_row_major(bitmap, populate_data, &reader);
        // record blackedges in blackedges bitmap
        track_blackedges(bitmap, blackedges);
        // remove the blackedges
        Bit2_map_row_major(bitmap, unblackedges, &blackedges);
        // print the unblackedged image to stdout
        printf("P1\n %d %d\n", data.width, data.height);
        Bit2_map_row_major(bitmap, print_image, NULL);
        
        if (input not_eq stdin) {
                fclose(input);
        }
        // free memory
        Pnmrdr_free(&reader);
        Bit2_free(&bitmap);
        Bit2_free(&blackedges);
        return EXIT_SUCCESS;
}

/********** populate_data ********
 *
 * Description: reads bits from the given input file using Pnmrdr and populates
 *              a Bit2 instance with the bits from the file
 *
 * Parameters: 
 *          int col: the column of the current bit
 *          int row: the row of the current bit
 *          Bit2_T bitmap: given bitmap
 *          int bit: value of the current bit
 *          void *cl: pointer to Pnmrdr_T instance
 *      
 * Return: none
 *      
 * Expects: bitmap is not NULL, col and row are within bounds, *cl points to a
 *          Pnmrdr_T
 *
 * Notes: apply function called by Bit2_map_row_major or Bit2_map_col_major
 *      
 ************************/
void populate_data(int col, int row, Bit2_T bitmap, int bit, void *cl) 
{
        (void) bit;
        Pnmrdr_T *reader_p = cl;
        // read next bit into bit2
        Bit2_put(bitmap, col, row, Pnmrdr_get(*reader_p)); 
}

/********** track_blackedges ********
 *
 * Description: checks if all edge pixels are black edges, then uses BFS on the 
 *              given bitmap to track which pixels are black edges
 *
 * Parameters: 
 *          Bit2_T bitmap: the given bitmap of the image
 *          Bit2_T blackedges: bitmap containing information on which pixels are
 *                             black edges (1 = black edge, 0 = normal pixel)
 *      
 * Return: none
 *      
 * Expects: bitmap and blackedges are not NULL
 *
 * Notes: CRE if bitmap and blackedges are NULL
 *      
 ************************/
void track_blackedges(Bit2_T bitmap, Bit2_T blackedges) 
{
        assert(bitmap not_eq NULL and blackedges not_eq NULL);
        Seq_T pq = Seq_new(0);
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        // add all black edges at boundaries to queue
        for (int c = 0; c < width; c++) {
                enqueue_be(c, 0, bitmap, blackedges, pq);
                enqueue_be(c, height - 1, bitmap, blackedges, pq);
        }
        for (int r = 0; r < height; r++) {
                enqueue_be(0, r, bitmap, blackedges, pq);
                enqueue_be(width - 1, r, bitmap, blackedges, pq);
        }

        while (Seq_length(pq) > 0) {
                struct Pixel *pixel = Seq_remlo(pq); // remove next pixel
                // add potential black edges to queue
                enqueue_be(pixel->col - 1, pixel->row, bitmap, blackedges, pq);
                enqueue_be(pixel->col + 1, pixel->row, bitmap, blackedges, pq);
                enqueue_be(pixel->col, pixel->row - 1, bitmap, blackedges, pq);
                enqueue_be(pixel->col, pixel->row + 1, bitmap, blackedges, pq);
                free(pixel);
        }
        Seq_free(&pq);
}

/********** enqueue_be ********
 *
 * Description: identifies whether the given pixel is a black edge and adds the
 *              pixel to the BFS priority queue if it is a black edge
 *
 * Parameters: 
 *          int col: the column of the pixel to be potentially added
 *          int row: the row of the pixel to be potentially added
 *          Bit2_T bitmap: the given bitmap of the image
 *          Bit2_T blackedges: bitmap containing information on which pixels are
 *                             black edges (1 = black edge, 0 = normal pixel)
 *          Seq_T pq: the BFS priority queue
 *      
 * Return: none
 *      
 * Expects: bitmap and blackedges are not NULL (checked in track_blackedges), pq
 *          is not NULL
 *
 * Notes: CRE if pq is NULL
 *      
 ************************/
void enqueue_be(int col, int row, Bit2_T bitmap, Bit2_T blackedges, Seq_T pq)
{
        assert(pq not_eq NULL);
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        if (col >= 0 and col < width and row >= 0 and row < height) {
                bool visited = Bit2_get(blackedges, col, row) == 1;
                // found a black edge
                if (Bit2_get(bitmap, col, row) == 1 and visited == false) {
                        // update tracking bitmap
                        Bit2_put(blackedges, col, row, 1);
                        // create new Pixel instance and add to priority queue
                        struct Pixel *pixel = malloc(sizeof(*pixel));
                        assert(pixel not_eq NULL); // check malloc worked
                        pixel->col = col;
                        pixel->row = row;
                        Seq_addhi(pq, pixel);
                }
        }
}

/********** unblackedges ********
 *
 * Description: replaces current bit with a white pixel if it is a black edge
 *
 * Parameters: 
 *          int col: the column of the current bit
 *          int row: the row of the current bit
 *          Bit2_T bitmap: given bitmap
 *          int bit: value of the current bit
 *          void *cl: pointer to blackedges Bit2_T (tracking bitmap)
 *      
 * Return: none
 *      
 * Expects: *cl is a pointer to blackedges, the tracking bitmap
 *
 * Notes: apply function called by Bit2_map_row_major or Bit2_map_col_major
 *      
 ************************/
void unblackedges(int col, int row, Bit2_T bitmap, int bit, void *cl) 
{
        (void) bit;
        Bit2_T *blackedges = cl;
        if (Bit2_get(*blackedges, col, row) == 1) { // current bit is black edge
                Bit2_put(bitmap, col, row, 0); // remove black edge
        }
}

/********** print_image ********
 *
 * Description: prints the current bit in the modified bitmap, should be called
 *              in row major order
 *
 * Parameters: 
 *          int col: the column of the current bit
 *          int row: the row of the current bit
 *          Bit2_T bitmap: given bitmap
 *          int bit: value of the current bit
 *          void *cl: pointer to client supplied closure
 *      
 * Return: none
 *      
 * Expects: *cl is NULL
 *
 * Notes: apply function called by Bit2_map_row_major
 *      
 ************************/
void print_image(int col, int row, Bit2_T bitmap, int bit, void *cl) 
{
        (void) bit, (void) cl;
        int nextbit = Bit2_get(bitmap, col, row);
        printf("%d", nextbit);
}