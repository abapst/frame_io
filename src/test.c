#include "frameio.h"

int main(int argc, char **argv)
{
    (void) argc;
    int cnt = 0;
    const char *filename = argv[1];
    const char *imagename = argv[2];

    rgb binframe;
    rgb img;
    binframe.data = NULL;

    /* Read and resize a small input video, writing it to output.mp4 */
    FILE* in = fio_OpenReadStream(filename,100,100);
    FILE* out = NULL;

    fprintf(stderr,"Reading video %s\n",filename);
    while(fio_ReadFrame(&binframe, in)) {
        fprintf(stderr, "\r(%d)",++cnt);
        if(out == NULL) {
            out = fio_OpenWriteStream("output.mp4",binframe.h,binframe.w);
        }
        fio_WriteFrame(&binframe,out);
    }

    /* Read an image and write it out output.jpg */
    fprintf(stderr,"Reading image %s\n",imagename);
    fio_imread(imagename,&img,-1,-1);
    fio_imwrite("output.jpg",&img);
    fprintf(stderr,"Done.\n");

    /* Cleanup */
    free(img.data);
    fio_close(in);
    fio_close(out);
    return 0;
}
