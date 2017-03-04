from libc.stdlib cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t

import ctypes as c
import numpy as np
cimport numpy as np

cimport declarations as dec

cdef class VideoReader:

    cdef FILE* fin
    cdef FILE* fout
    cdef dec.rgb c_frame

    def __cinit__(self):
        self.fin = NULL
        self.fout = NULL
        self.c_frame.data = NULL

    def open(self, filename, mode, shape=(-1,-1)):
        cdef const char* c_filename = filename

        if mode is 'r':
            self.fin = dec.fio_OpenReadStream(c_filename, shape[0], shape[1])
        if mode is 'w':
            if self.c_frame.data != NULL:
                self.fout = dec.fio_OpenWriteStream(c_filename, self.c_frame.h, self.c_frame.w)

    def close(self):
            dec.fio_close(self.fin)
            dec.fio_close(self.fout)

    def readframe(self):
        assert self.fin != NULL
        cdef int retval;
        retval = dec.fio_ReadFrame(&self.c_frame, self.fin)
        
        # convert c buffer to numpy array
        data_ptr = c.cast(<uintptr_t>self.c_frame.data, c.POINTER(c.c_uint8))
        frame = np.ctypeslib.as_array(data_ptr, shape=(self.c_frame.h,self.c_frame.w,3))
        return (retval, frame)

    def writeframe(self, np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None):
        cdef dec.rgb c_frame
        c_frame.data = &frame[0,0,0]
        c_frame.h = frame.shape[0]
        c_frame.w = frame.shape[1]
        dec.fio_WriteFrame(&c_frame, self.fout)

def imread(filename, shape=(-1,-1)):
    cdef const char *c_filename = filename
    cdef dec.rgb c_frame
    dec.fio_imread(c_filename, &c_frame, shape[0], shape[1])

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_frame.data, c.POINTER(c.c_uint8))
    frame = np.ctypeslib.as_array(data_ptr, shape=(c_frame.h,c_frame.w,3))
    return frame

def imwrite(np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None, filename):
    cdef const char *c_filename = filename
    cdef dec.rgb c_frame
    c_frame.data = &frame[0,0,0]
    c_frame.h = frame.shape[0]
    c_frame.w = frame.shape[1]
    dec.fio_imwrite(c_filename, &c_frame)

def imresize(np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None, scale=-1, shape=(-1,-1), mode="bilinear"):
    cdef const char *c_mode = mode
    cdef dec.rgb c_in
    cdef dec.rgb c_out
    cdef int rows
    cdef int cols

    if scale != -1:
        assert scale > 0
        rows = scale*frame.shape[0]
        cols = scale*frame.shape[1]
    else:
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
    
    dec.imresize(&c_in, &c_out, rows, cols, c_mode)

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    frame_resized = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,3))
    return frame_resized
