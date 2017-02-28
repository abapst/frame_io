#include "frameio.h"

int main(int argc, char **argv)
{
    int cnt = 0;
    const char *filename = argv[1];
    const char *imagename = argv[2];
    rgb binframe;
    rgb img;
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

    fprintf(stderr,"Reading image %s\n",imagename);
    fio_imread(imagename,&img,-1,-1);
    fio_imwrite("output.jpg",&img);

    /* Cleanup */
    free(img.data);
    fio_close(in);
    fio_close(out);
    return 0;
}
