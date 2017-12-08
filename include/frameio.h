#ifndef FRAMEIO_H
#define FRAMEIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "image.h"

#define NCHANNELS 3

/* Function declarations */
FILE* fio_OpenReadStream(const char *filename,int rows, int cols);
FILE* fio_OpenWriteStream(const char* filename,int rows,int cols);

int fio_ReadFrame(rgb *binframe, FILE *in);
int fio_WriteFrame(rgb *binframe, FILE *out);

void fio_close(FILE* fd);

int fio_imread(const char* filename, rgb *binframe, int rows, int cols);
int fio_imwrite(const char* filename, rgb *binframe);

#endif
