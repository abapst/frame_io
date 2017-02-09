#ifndef FRAMEIO_H
#define FRAMEIO_H

typedef struct _rgb
{
    int w;
    int h;
    unsigned char *data;
} rgb;

// Function declarations
FILE* fio_OpenReadStream(const char *filename);
FILE* fio_OpenWriteStream(const char* filename,int rows,int cols);

void fio_WriteFrame(rgb *binframe, FILE *out);
rgb *fio_ReadFrame(rgb *binframe, FILE *in);

void fio_CloseReadStream(FILE* in);
void fio_CloseWriteStream(FILE* out);
#endif
