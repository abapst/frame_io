#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>

#include "frameio.h"

FILE* fio_OpenReadStream(const char* filename)
{
    FILE *in = NULL;

    char buffer[1024];

    // Assemble ffmpeg command
    snprintf(buffer,sizeof(buffer),"ffmpeg -i %s \
                                   -f image2pipe -loglevel quiet \
                                   -hide_banner -vcodec ppm -",\
                                   filename);

    in = popen(buffer,"r");
    if (in == NULL){
        fputs("read pipe open failed!\n",stderr);
        exit(-1); 
    }
    return in;
}

FILE* fio_OpenWriteStream(const char* filename,int rows,int cols)
{

    FILE *out = NULL;

    char buffer[1024];
    char rows_buffer[8];
    char cols_buffer[8];

    // Release symlinks to filename, if any
    unlink(filename);

    // Convert rows and cols to string 
    sprintf(rows_buffer,"%d",rows);
    sprintf(cols_buffer,"%d",cols);

    // Assemble ffmpeg command
    snprintf(buffer,sizeof(buffer),"ffmpeg -f rawvideo \
                                   -framerate 30 -pixel_format rgb24 \
                                   -video_size %s*%s \
                                   -i - -b:v 7000k %s \
                                   2> /dev/null", \
                                   cols_buffer,rows_buffer,filename);

    out = popen(buffer,"w");   
    if (out == NULL){
        fputs("write pipe open failed!\n",stderr);
        exit(-1); 
    }
    return out;
}

int fio_ReadFrame(rgb *binframe, FILE *in)
{
    char string[81];
    int width, height;

    // Read exactly one PPM formatted frame from input pipe
    while(fgets(string,80,in) != NULL) {
        // read image header
        if(strncmp(string, "P6\n", 3)) {
            fprintf(stderr, "Frame reader out of sync. Game over.\n");
            fprintf(stderr, "String was: %s", string);
            exit(-1);
        }
        fgets(string, 80, in);
        sscanf(string,"%d %d", &width, &height);
        fgets(string, 80, in);
        // Allocate frame data if this is the first frame
        if(binframe->data == NULL) {
            binframe->data = (unsigned char *)malloc(width*height*3);
            binframe->w = width;
            binframe->h = height;
            if(binframe->data == NULL) {
                fputs("malloc failed!\n", stderr);
                exit(-1);
            }
        }

        // Read frame
        fread(binframe->data, width*height*3, sizeof(unsigned char), in);
        return 1;
    }
    
    // Nothing more to read
    free(binframe->data);
    return 0;
}

void fio_WriteFrame(rgb *binframe, FILE *out)
{
    fwrite(binframe->data,binframe->h*binframe->w*3,sizeof(unsigned char),out);
}

void fio_CloseReadStream(FILE* in)
{
    pclose(in);
}

void fio_CloseWriteStream(FILE* out)
{
    pclose(out);
}
