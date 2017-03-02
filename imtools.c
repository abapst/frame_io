#include "imtools.h"

/* 
 * Image resizing using bilinear interpolation
 */
void imresize(rgb *input, rgb *output, int hout, int wout)
{
    int ii, jj, kk, x, y, a, b, c, d, index, val;
    int hin = input->h;
    int win = input->w; 
    
    output->h = hout;
    output->w = wout;
    output->data = malloc(wout*hout*NCHANNELS);

    float x_ratio = (float)(win-1)/wout;
    float y_ratio = (float)(hin-1)/hout;
    float x_diff, y_diff;

    for (ii = 0; ii < hout; ii++) {
        for (jj = 0; jj < wout; jj++) {
            x = (int)(x_ratio*jj);
            y = (int)(y_ratio*ii);
            x_diff = (x_ratio*jj)-x;
            y_diff = (y_ratio*ii)-y;

            for (kk = 0; kk < NCHANNELS; kk++) {
                index = y*win*NCHANNELS+x*NCHANNELS+kk;
                a = input->data[index];
                b = input->data[index+NCHANNELS];
                c = input->data[index+NCHANNELS*win];
                d = input->data[index+NCHANNELS*win+NCHANNELS];

                val = (int)(a*(1-x_diff)*(1-y_diff) + 
                            b*(x_diff)*(1-y_diff) +
                            c*(y_diff)*(1-x_diff) + 
                            d*(x_diff*y_diff));

                output->data[ii*wout*NCHANNELS+jj*NCHANNELS+kk] = val;
            }
        }
    }
}
