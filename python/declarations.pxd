from libc.stdlib cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t

cdef extern from "../frameio.h":
    cdef struct _rgb:
        int w
        int h
        unsigned char *data 
    ctypedef _rgb rgb

    FILE* fio_OpenWriteStream(const char* filename, int rows, int cols)
    FILE* fio_OpenReadStream(const char *filename, int rows, int cols)

    void fio_WriteFrame(rgb *binframe, FILE *fout)
    int fio_ReadFrame(rgb *binframe, FILE *fin)

    void fio_close(FILE* fout)

    int fio_imread(const char *filename, rgb *binframe, int rows, int cols)
    void fio_imwrite(const char *filename, rgb *binframe)

cdef extern from "../imtools.h":
    void imresize(rgb *c_input, rgb *c_output, int hout, int wout, const char *alg);
