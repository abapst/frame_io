#include "image.h"

void image_alloc(rgb *image, int h, int w, int c)
{
    image->h = h;
    image->w = w;
    image->c = c;
    image->data = malloc(h*w*c);
}
