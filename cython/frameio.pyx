from libc.stdlib cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t

import ctypes as c
import numpy as np
cimport numpy as np

# Declare external C functions
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

cdef class VideoReader:

    cdef FILE* fin
    cdef FILE* fout
    cdef rgb c_frame

    def __cinit__(self):
        self.fin = NULL
        self.fout = NULL
        self.c_frame.data = NULL

    def open(self, filename, mode, shape=(-1,-1)):
        cdef const char* c_filename = filename

        if mode is 'r':
            self.fin = fio_OpenReadStream(c_filename, shape[0], shape[1])
        if mode is 'w':
            if self.c_frame.data != NULL:
                self.fout = fio_OpenWriteStream(c_filename, self.c_frame.h, self.c_frame.w)

    def close(self):
            fio_close(self.fin)
            fio_close(self.fout)

    def readframe(self):
        assert self.fin != NULL
        cdef int retval;
        retval = fio_ReadFrame(&self.c_frame, self.fin)
        
        # convert c buffer to numpy array
        data_ptr = c.cast(<uintptr_t>self.c_frame.data, c.POINTER(c.c_uint8))
        frame = np.ctypeslib.as_array(data_ptr, shape=(self.c_frame.h,self.c_frame.w,3))
        return (retval, frame)

    def writeframe(self, np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None):
        cdef rgb c_frame
        c_frame.data = &frame[0,0,0]
        c_frame.h = frame.shape[0]
        c_frame.w = frame.shape[1]
        fio_WriteFrame(&c_frame, self.fout)

def imread(filename, shape=(-1,-1)):
    cdef const char *c_filename = filename
    cdef rgb c_frame
    fio_imread(c_filename, &c_frame, shape[0], shape[1])

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_frame.data, c.POINTER(c.c_uint8))
    frame = np.ctypeslib.as_array(data_ptr, shape=(c_frame.h,c_frame.w,3))
    return frame

def imwrite(np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None, filename):
    cdef const char *c_filename = filename
    cdef rgb c_frame
    c_frame.data = &frame[0,0,0]
    c_frame.h = frame.shape[0]
    c_frame.w = frame.shape[1]
    fio_imwrite(c_filename, &c_frame)

def imresize(np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None, shape=(-1,-1), mode="bilinear"):
    cdef const char *c_mode = mode
    cdef rgb c_in
    cdef rgb c_out
    cdef int rows
    cdef int cols

    if shape[0] == -1:
        rows = frame.shape[0]
    else:
        rows = shape[0]
    if shape[1] == -1:
        cols = frame.shape[1]
    else:
        cols = shape[1]

    c_in.data = &frame[0,0,0]
    c_in.h = frame.shape[0]
    c_in.w = frame.shape[1]
    
    imresize(&c_in, &c_out, rows, cols, c_mode)

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    frame_resized = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,3))
    return frame_resized
