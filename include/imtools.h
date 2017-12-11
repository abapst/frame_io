#ifndef IMTOOLS_H
#define IMTOOLS_H

#include <math.h>
#include "frameio.h"

#define GRAYLEVEL_8BIT    256

void imresize(rgb *input, rgb *output, int hout, int wout, const char *alg);
void imresize_nearest(rgb *input, rgb *output, int hout, int wout);
void imresize_bilinear(rgb *input, rgb *output, int hout, int wout);

int rgb2gray(rgb *input, rgb *output);
int gray2rgb(rgb *input, rgb *output);

int equalizeHist(rgb *input, rgb *output);

#endif
