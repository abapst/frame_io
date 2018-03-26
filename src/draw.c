#include "draw.h"

void draw_box_one_pixel(rgb *im, int x1, int y1, int x2, int y2, int r,int g, int b)
{
    int i,idx1,idx2;

    for (i = x1; i <= x2; i++) {
        idx1 = i*NCHANNELS + y1*im->w*NCHANNELS;
        idx2 = i*NCHANNELS + y2*im->w*NCHANNELS;

        im->data[idx1] = r;
        im->data[idx2] = r;

        im->data[idx1 + 1] = g;
        im->data[idx2 + 1] = g;

        im->data[idx1 + 2] = b;
        im->data[idx2 + 2] = b;
    }

    for (i = y1; i <= y2; i++) {
        idx1 = x1*NCHANNELS + i*im->w*NCHANNELS;
        idx2 = x2*NCHANNELS + i*im->w*NCHANNELS;

        im->data[idx1] = r;
        im->data[idx2] = r;

        im->data[idx1 + 1] = g;
        im->data[idx2 + 1] = g;

        im->data[idx1 + 2] = b;
        im->data[idx2 + 2] = b;
    }
}


void draw_box(rgb *im, int x1, int y1, int x2, int y2, int thickness,
              int r, int g, int b)
{
    int i;
    for (i = 0; i < thickness; i++) {
        x1 = x1 < 0 ? 0 : x1;
        x1 = x1 >= im->w ? im->w-1 : x1;
        x2 = x2 < 0 ? 0 : x2;
        x2 = x2 >= im->w ? im->w-1 : x2;
        y1 = y1 < 0 ? 0 : y1;
        y1 = y1 >= im->h ? im->h-1 : y1;
        y2 = y2 < 0 ? 0 : y2;
        y2 = y2 >= im->h ? im->h-1 : y2;
        draw_box_one_pixel(im,x1+i,y1+i,x2-i,y2-i,r,g,b);
    }
}

void draw_marker(rgb *im, int x, int y, int size, int r, int g, int b)
{
    int n_boxes = size/2;

    int i, x1, x2, y1, y2;
    for (i = 0; i < n_boxes; i++) {
        x1 = x - i;
        x2 = x + i;
        y1 = y - i;
        y2 = y + i;
        x1 = x1 < 0 ? 0 : x1;
        x1 = x1 >= im->w ? im->w-1 : x1;
        x2 = x2 < 0 ? 0 : x2;
        x2 = x2 >= im->w ? im->w-1 : x2;
        y1 = y1 < 0 ? 0 : y1;
        y1 = y1 >= im->h ? im->h-1 : y1;
        y2 = y2 < 0 ? 0 : y2;
        y2 = y2 >= im->h ? im->h-1 : y2;
        draw_box_one_pixel(im,x1,y1,x2,y2,r,g,b);
    }
}
