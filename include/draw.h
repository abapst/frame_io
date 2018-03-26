#ifndef DRAW_H
#define DRAW_H

#include "frameio.h"

void draw_box_one_pixel(rgb *im, int x1, int y1, int x2, int y2,
                        int r,int g, int b);

void draw_box(rgb *im, int x1, int y1, int x2, int y2,
              int thickness, int r,int g, int b);

void draw_marker(rgb *im, int x, int y, int size, int r, int g, int b);

#endif
