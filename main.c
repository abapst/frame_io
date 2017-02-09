#include <stdlib.h>
#include <stdio.h>

#include "frameio.h"

int main(int argc, char **argv)
{
    const char *filename = argv[1];
    int cnt = 0;
    rgb *binframe = NULL;

    FILE* in = fio_OpenReadStream(filename);
    FILE* out = NULL;

    fprintf(stderr,"Reading file %s\n",filename);
    while((binframe = fio_ReadFrame(binframe,in)) != NULL) {
        fprintf(stderr, "\r(%d)",++cnt);
        if(out == NULL) {
            out = fio_OpenWriteStream("output.mp4",binframe->h,binframe->w);
        }
        fio_WriteFrame(binframe,out);
    }
    fprintf(stderr,"\n");

    fio_CloseReadStream(in);
    fio_CloseWriteStream(out);
    return 0;
}