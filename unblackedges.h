#ifndef UNBLACKEDGES_H
#define UNBLACKEDGES_H_H

#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <table.h>
#include <atom.h>
#include <ctype.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "bit2.h"
void populate_data(int col, int row, Bit2_T bit2, int bit, void *cl);
void unblackedges(int col, int row, Bit2_T bitmap, int bit, void *cl);
//bool is_blackedge(int col, int row, Bit2_T bitmap, Bit2_T blackedges);
void track_blackedges(Bit2_T bitmap, Bit2_T blackedges);
void print_image(int col, int row, Bit2_T bitmap, int bit, void *cl);

#endif