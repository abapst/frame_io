#include "image.h"
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

    image_alloc(output, hout, wout, 3);

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


int rgb2gray(rgb *input, rgb *output)
{
    if (input == NULL || output == NULL) {
        return -1;
    }

    if (input->c != 3) {
        return -1;
    }

    int nrows = input->h;
    int ncols = input->w;

    image_alloc(output, nrows, ncols, 1);

    float avg = 0.;
    int i,j;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            int idx = ncols*i*NCHANNELS + j*NCHANNELS;
            avg = (input->data[idx] +
                   input->data[idx + 1]+
                   input->data[idx + 2])/3;
            avg = roundf(avg);
            output->data[ncols*i + j] = (unsigned char)avg;
        }
    }
    return 0;
}


int gray2rgb(rgb *input, rgb *output)
{
    if (input == NULL || output == NULL) {
        return -1;
    }

    if (input->c != 1) {
        return -1;
    }

    int nrows = input->h;
    int ncols = input->w;

    image_alloc(output, nrows, ncols, 3);

    int i,j;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            unsigned char val = input->data[ncols*i + j];
            int idx = ncols*i*NCHANNELS + j*NCHANNELS;
            output->data[idx] = val;
            output->data[idx + 1] = val;
            output->data[idx + 2] = val;
        }
    }
    return 0;
}


int equalizeHist(rgb *input, rgb *output)
{
    if (input == NULL || output == NULL) {
        return -1;
    }

    if (input->c != 1) {
        return -1;
    }

    int i,j,idx;
    unsigned long long *hist = calloc(GRAYLEVEL_8BIT,sizeof(unsigned long long));

    int nrows = input->h;
    int ncols = input->w;
    image_alloc(output, nrows, ncols, 1);

    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            idx = ncols*i + j;
            hist[input->data[idx]]++;
        }
    }

    double *cdf = malloc(GRAYLEVEL_8BIT*sizeof(double));
    cdf[0] = 255*(double)hist[0]/(nrows*ncols);
    for (i = 0; i < GRAYLEVEL_8BIT; i++) {
        cdf[i] = 255*(double)hist[i]/(nrows*ncols) + cdf[i-1];
    }

    double cdfmin = 0;
    for (i = 0; i < GRAYLEVEL_8BIT; i++) {
        if (cdf[i] > 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    double val;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            idx = ncols*i + j;
            val = roundf(cdf[input->data[idx]]-cdfmin);
            output->data[idx] = (unsigned char)val;
        }
    }

    free(hist);
    free(cdf);
    return 0;
}
