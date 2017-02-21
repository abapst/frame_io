#include <stdlib.h>
#include <stdio.h>

#include "frameio.h"

int main(int argc, char **argv)
{
    int cnt = 0;
    const char *filename = argv[1];
    rgb binframe;
    binframe.data = NULL;

    FILE* in = fio_OpenReadStream(filename,-1,-1);
    FILE* out = NULL;

    fprintf(stderr,"Reading file %s\n",filename);
    while(fio_ReadFrame(&binframe, in)) {
        fprintf(stderr, "\r(%d)",++cnt);
        if(out == NULL) {
            out = fio_OpenWriteStream("output.mp4",binframe.h,binframe.w);
        }
        fio_WriteFrame(&binframe,out);
    }
    fprintf(stderr,"\n");

    fio_CloseReadStream(in);
    fio_CloseWriteStream(out);
    return 0;
}
