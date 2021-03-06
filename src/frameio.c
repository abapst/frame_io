#include "frameio.h"

/*
 * Open an input stream from a video or image file using ffmpeg.
 * If rows = -1 or cols = -1, the files are read at their native
 * height/width in the respective dimension, otherwise the frames
 * are scaled accordingly.
 */
FILE* fio_OpenReadStream(const char* filename, int rows, int cols)
{
    FILE *in = NULL;

    char buffer[1024];

    snprintf(buffer,sizeof(buffer),"ffmpeg -i %s -vf scale=%d:%d \
                                   -f image2pipe -loglevel quiet \
                                   -hide_banner -vcodec ppm -",\
                                   filename,cols,rows);

    in = popen(buffer,"r");
    if (in == NULL) {
        fprintf(stderr,"fio_OpenReadStream error\n");
        return NULL;
    }
    return in;
}


/*
 * Open an output stream to a video or image file using ffmpeg.
 */
FILE* fio_OpenWriteStream(const char* filename,int rows,int cols)
{

    FILE *out = NULL;

    char buffer[1024];
    char rows_buffer[8];
    char cols_buffer[8];

    /* Release symlinks to filename, if any. */
    unlink(filename);

    sprintf(rows_buffer,"%d",rows);
    sprintf(cols_buffer,"%d",cols);

    snprintf(buffer,sizeof(buffer),"ffmpeg -f rawvideo \
                                   -framerate 30 -pixel_format rgb24 \
                                   -video_size %s*%s \
                                   -i - -b:v 7000k %s \
                                   2> /dev/null", \
                                   cols_buffer,rows_buffer,filename);

    out = popen(buffer,"w");
    if (out == NULL){
        fprintf(stderr,"fio_OpenWriteStream error\n");
        return NULL;
    }
    return out;
}


/* Read exactly one frame from an open ffmpeg stream. Returns -1
 * on error, 1 on success, and 0 if there are no more frames to
 * read.
 */
int fio_ReadFrame(rgb *binframe, FILE *in)
{
    char string[81];
    int width, height;
    size_t nbytes;

    /* Read exactly one PPM formatted frame from input pipe */
    while(fgets(string,80,in) != NULL) {
        if(strncmp(string, "P6\n", 3)) {
            fprintf(stderr, "fio_ReadFrame: Incorrect PPM magic number.\n");
            fprintf(stderr, "String was: %s", string);
            return -1;
        }

        if(fgets(string, 80, in) == NULL) {
            fprintf(stderr, "fio_ReadFrame: Frame header read error.\n");
            return -1;
        }

        sscanf(string,"%d %d", &width, &height);

        if(fgets(string, 80, in) == NULL) {
            fprintf(stderr, "fio_ReadFrame: Frame header read error.\n");
            return -1;
        }

        if(binframe->data == NULL) {
            binframe->data = (unsigned char *)malloc(width*height*3);
            binframe->w = width;
            binframe->h = height;
            if(binframe->data == NULL) {
                fprintf(stderr, "fio_ReadFrame: malloc failed!\n");
                return -1;
            }
        }

        nbytes = fread(binframe->data, sizeof(unsigned char), height*width*3, in);
        if ((int)nbytes < height*width*3) {
            fprintf(stderr, "fio_ReadFrame: fread error.\n");
            return -1;
        }

        return 1;
    }

    return 0;
}


/*
 * Write exactly one frame to an ffmpeg output stream.
 */
int fio_WriteFrame(rgb *binframe, FILE *out)
{
    size_t nbytes;
    nbytes = fwrite(binframe->data,\
                    sizeof(unsigned char),\
                    binframe->h*binframe->w*3,\
                    out);

    if ((int)nbytes < binframe->h*binframe->w*3) {
        fprintf(stderr, "fio_WriteFrame: fwrite error.\n");
        return -1;
    }

    return 0;
}


/*
 * Close an ffmpeg input/output stream.
 */
void fio_close(FILE* fd)
{
    pclose(fd);
}


/*
 * Read a single image using ffmpeg. The image can be read at native size
 * (rows = -1 or cols = -1) or scaled to a different size using ffmpeg's
 * scaling option.
 */
int fio_imread(const char* filename, rgb *binframe, int rows, int cols)
{
    FILE* in = fio_OpenReadStream(filename, rows, cols);
    binframe->data = NULL;
    if (fio_ReadFrame(binframe, in) == -1) {
        return -1;
    }
    fio_close(in);
    return 0;
}


/*
 * Write a single image to an output file using ffmpeg.
 */
int fio_imwrite(const char* filename, rgb *binframe)
{
    FILE *out;
    if (NULL == (out = fio_OpenWriteStream(filename,\
                                   binframe->h,\
                                   binframe->w))) {
        return -1;
    }
    fio_WriteFrame(binframe, out);
    fio_close(out);
    return 0;
}
