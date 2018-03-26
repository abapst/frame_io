from libc.stdlib cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t

cdef extern from "../include/frameio.h":
    cdef struct _rgb:
        int w
        int h
        int c
        unsigned char *data
    ctypedef _rgb rgb

    FILE* fio_OpenWriteStream(const char* filename, int rows, int cols)
    FILE* fio_OpenReadStream(const char *filename, int rows, int cols)

    int fio_ReadFrame(rgb *binframe, FILE *fin)
    int fio_WriteFrame(rgb *binframe, FILE *fout)

    void fio_close(FILE* fout)

    int fio_imread(const char *filename, rgb *binframe, int rows, int cols)
    int fio_imwrite(const char *filename, rgb *binframe)

cdef extern from "../include/imtools.h":
    void imresize(rgb *c_input, rgb *c_output, int hout, int wout, const char *alg);
    int rgb2gray(rgb *c_input, rgb *c_output);
    int gray2rgb(rgb *c_input, rgb *c_output);
    int equalizeHist(rgb *c_input, rgb *c_output);

cdef extern from "../include/draw.h":
    void draw_box(rgb *im, int x1, int y1, int x2, int y2, int thickness, int r,int g, int b);
    void draw_marker(rgb *im, int x, int y, int size, int r,int g, int b);

cdef extern from "../include/image.h":
    void image_alloc(rgb *image, int h, int w, int c);
