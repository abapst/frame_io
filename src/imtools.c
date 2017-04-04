#include "imtools.h"

/*
 * Image resizing wrapper
 */
void imresize(rgb *input, rgb *output, int hout, int wout, const char *alg)
{
    if (!hout || !wout) {
        fprintf(stderr,"imresize: output dimensions can't be 0!");
        exit(-1);
    }

    output->h = hout;
    output->w = wout;
    output->data = malloc(wout*hout*NCHANNELS);

    /* Just copy the image if the size doesn't change */
    if (hout == input->h && wout == input->w) {
        memcpy(output->data,input->data,input->h*input->w*NCHANNELS);
    }

    if (!strcmp(alg,"nearest")) {
        imresize_nearest(input, output, hout, wout);
    }
    if (!strcmp(alg,"bilinear")) {
        imresize_bilinear(input, output, hout, wout);
    }
}

/* 
 * Image resizing using bilinear interpolation
 */
void imresize_bilinear(rgb *input, rgb *output, int hout, int wout)
{
    int ii, jj, kk, x, y, a, b, c, d, index, val;
    int hin = input->h;
    int win = input->w; 

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

/*
 * Image resizing using nearest-neighbor interpolation
 */
void imresize_nearest(rgb *input, rgb *output, int hout, int wout)
{
    int ii, jj, x, y, idx_in, idx_out;
    int hin = input->h;
    int win = input->w;

    float x_ratio = (float)win/wout;
    float y_ratio = (float)hin/hout;

    for (ii = 0; ii < hout; ii++) {
        for (jj = 0; jj < wout; jj++) {
            x = (int)(x_ratio*jj);
            y = (int)(y_ratio*ii);
            idx_in = y*win*NCHANNELS+x*NCHANNELS;
            idx_out = ii*wout*NCHANNELS+jj*NCHANNELS;

            output->data[idx_out] = input->data[idx_in];
            output->data[idx_out+1] = input->data[idx_in+1];
            output->data[idx_out+2] = input->data[idx_in+2];
        }
    }
}
