#ifndef IMTOOLS_H
#define IMTOOLS_H

#include "frameio.h"

void imresize(rgb *input, rgb *output, int hout, int wout, const char *alg);
void imresize_nearest(rgb *input, rgb *output, int hout, int wout);
void imresize_bilinear(rgb *input, rgb *output, int hout, int wout);

#endif
