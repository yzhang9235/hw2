/**************************************************************
 *
 *                     unblackedges.h
 *
 *     Assignment: hw2
 *     Authors:  Lauren O'Rourke (lorour02), Yining Zhang (yzhang92)
 *     Date:     1/31/2026
 *
 *     Summary:
 *     This header file provides an interface for removing black edges from a
 *     provided portable bitmap image. It contains functions used to read bits
 *     into a Bit2 instance, help identify black edges, remove black edges, and
 *     output a new portable bitmap with black edges removed.
 *
 **************************************************************/
#ifndef UNBLACKEDGES_H
#define UNBLACKEDGES_H

#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <pnmrdr.h>
#include <seq.h>
#include "bit2.h"

void populate_data(int col, int row, Bit2_T bitmap, int bit, void *cl);
void unblackedges(int col, int row, Bit2_T bitmap, int bit, void *cl);
bool is_blackedge(int col, int row, Bit2_T bitmap, Bit2_T blackedges);
void track_blackedges(Bit2_T bitmap, Bit2_T blackedges);
void print_image(int col, int row, Bit2_T bitmap, int bit, void *cl);
void enqueue_be(int col, int row, Bit2_T bitmap, Bit2_T blackedges, Seq_T pq);

#endif
