#ifndef IMAGE_H
#define IMAGE_H

typedef struct _rgb
{
    int w;
    int h;
    int c;
    unsigned char *data;
} rgb;

void image_alloc(rgb *image, int h, int w, int c);

#endif
