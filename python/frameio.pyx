from libc.stdlib cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t

import ctypes as c
import os
import errno
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
        cdef bytes filename_bytes = filename.encode()
        cdef const char* c_filename = filename_bytes

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
        c_frame.c = 3
        dec.fio_WriteFrame(&c_frame, self.fout)

def imread(filename, shape=(-1,-1)):
    try:
        f = open(filename)
        f.close()
    except:
        raise

    cdef bytes filename_bytes = filename.encode()
    cdef const char *c_filename = filename_bytes
    cdef dec.rgb c_frame
    dec.fio_imread(c_filename, &c_frame, shape[0], shape[1])

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_frame.data, c.POINTER(c.c_uint8))
    frame = np.ctypeslib.as_array(data_ptr, shape=(c_frame.h,c_frame.w,3))
    return frame


def imwrite(frame, filename):

    if frame.ndim != 3 or frame.shape[2] == 1:
        print("[ERROR] imwrite: image dimensions should be h x w x 3!")
        return

    cdef np.ndarray[np.uint8_t,ndim=3,mode="c"] im_in = np.ascontiguousarray(frame, dtype=np.uint8)

    cdef bytes filename_bytes = filename.encode()
    cdef const char *c_filename = filename_bytes
    cdef dec.rgb c_frame
    c_frame.data = &im_in[0,0,0]
    c_frame.h = im_in.shape[0]
    c_frame.w = im_in.shape[1]
    c_frame.c = 3
    dec.fio_imwrite(c_filename, &c_frame)


def imresize(np.ndarray[np.uint8_t,ndim=3,mode="c"] frame not None, scale=-1, shape=(-1,-1), mode="bilinear"):
    cdef bytes mode_bytes = mode.encode()
    cdef const char *c_mode = mode_bytes
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
    c_in.c = 3

    dec.imresize(&c_in, &c_out, rows, cols, c_mode)

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    frame_resized = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,3))
    return frame_resized


def draw_box(im, pt1, pt2, color, thickness=1):
    cdef dec.rgb c_in
    cdef int x1 = pt1[0]
    cdef int y1 = pt1[1]
    cdef int x2 = pt2[0]
    cdef int y2 = pt2[1]
    cdef int r = color[0]
    cdef int g = color[1]
    cdef int b = color[2]
    cdef int t = thickness

    cdef np.ndarray[np.uint8_t,ndim=3,mode="c"] im_in = np.ascontiguousarray(im, dtype=np.uint8)

    c_in.data = &im_in[0,0,0]
    c_in.h = im_in.shape[0]
    c_in.w = im_in.shape[1]
    c_in.c = 3

    dec.draw_box(&c_in,x1,y1,x2,y2,t,r,g,b)

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_in.data, c.POINTER(c.c_uint8))
    im = np.ctypeslib.as_array(data_ptr, shape=(c_in.h,c_in.w,3))
    return im


def rgb2gray(im):
    if im.ndim != 3:
        print("[ERROR] rgb2gray: input image should have 3 dimensions!")
        return im

    cdef np.ndarray[np.uint8_t,ndim=3,mode="c"] im_in = np.ascontiguousarray(im, dtype=np.uint8)

    cdef dec.rgb c_in
    cdef dec.rgb c_out
    c_in.data = &im_in[0,0,0]
    c_in.h = im_in.shape[0]
    c_in.w = im_in.shape[1]
    c_in.c = 3

    dec.rgb2gray(&c_in, &c_out);

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    im = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,1))

    return im


def gray2rgb(im):
    if im.ndim < 2 or im.ndim > 3:
        print("[ERROR] gray2rgb: input image should have 2 or 3 dimensions!")
        return im

    if im.ndim == 2:
        im = np.expand_dims(im,2)
    else:
        if im.shape[2] != 1:
            print("[ERROR] gray2rgb: input image cannot have a third dimension with length > 1!")
            return im

    cdef np.ndarray[np.uint8_t,ndim=3,mode="c"] im_in = np.ascontiguousarray(im, dtype=np.uint8)

    cdef dec.rgb c_in
    cdef dec.rgb c_out
    c_in.data = &im_in[0,0,0]
    c_in.h = im_in.shape[0]
    c_in.w = im_in.shape[1]
    c_in.c = 1

    dec.gray2rgb(&c_in, &c_out);

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    im = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,3))

    return im


def equalizeHist(im):

    cdef np.ndarray[np.uint8_t,ndim=3,mode="c"] im_in = np.ascontiguousarray(im, dtype=np.uint8)

    cdef dec.rgb c_in
    cdef dec.rgb c_out
    c_in.data = &im_in[0,0,0]
    c_in.h = im_in.shape[0]
    c_in.w = im_in.shape[1]
    c_in.c = 1

    dec.equalizeHist(&c_in, &c_out);

    # convert c buffer to numpy array
    data_ptr = c.cast(<uintptr_t>c_out.data, c.POINTER(c.c_uint8))
    im = np.ctypeslib.as_array(data_ptr, shape=(c_out.h,c_out.w,1))

    return im
