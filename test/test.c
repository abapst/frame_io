#include "frameio.h"
#include "imtools.h"
#include "draw.h"

#define SCALE 2.345
#define NTEST 5

int test_imread(const char *image_file)
{
    rgb im;
    if (fio_imread(image_file, &im, -1, -1) < 0) {
        return -1;
    }

    if (fio_imwrite("image_out.jpg", &im) < 0) {
        return -1;
    }
    free(im.data);
    return 0;
}


int test_vidread(const char *video_file)
{
    int cnt = 0;
    rgb binframe = {0,0,0,NULL};
    FILE *in = 0;
    FILE *out = 0;
    if ((in = fio_OpenReadStream(video_file,100,100)) == NULL) {
        return -1;
    }

    while(fio_ReadFrame(&binframe, in)) {
        fprintf(stderr, "\r(%d)",++cnt);
        if(out == NULL) {
            if (NULL == (out = fio_OpenWriteStream("video_out.mp4",\
                                                   binframe.h,\
                                                   binframe.w))) {
                return -1;
            }
        }
        fio_WriteFrame(&binframe,out);
    }
    fprintf(stderr, "\r\r");

    free(binframe.data);
    fio_close(in);
    fio_close(out);
    return 0;
}


int test_draw_box(const char *image_file)
{
    rgb im;
    if (fio_imread(image_file, &im, -1, -1) < 0) {
        return -1;
    }

    draw_box(&im,20,20,100,100,10,255,0,0);
    draw_box(&im,40,40,120,120,10,0,255,0);
    draw_box(&im,60,60,140,140,10,0,0,255);

    if (fio_imwrite("image_out.jpg", &im) < 0) {
        return -1;
    }

    free(im.data);
    return 0;
}


int main()
{
    int pass = 0;
    const char *jpg = "data/bear.jpg";
    const char *bmp = "data/bear.bmp";
    const char *png = "data/bear.png";
    const char *mp4 = "data/cars.mp4";

    fprintf(stderr,"Running tests...\n");
    fprintf(stderr,"Test 1: Read/Write .jpg --->");
    if (test_imread(jpg) < 0) {
        fprintf(stderr," Failed\n");
    } else {
        fprintf(stderr," Success\n");
        pass++;
    }

    fprintf(stderr,"Test 2: Read/Write .bmp --->");
    if (test_imread(bmp) < 0) {
        fprintf(stderr," Failed\n");
    } else {
        fprintf(stderr," Success\n");
        pass++;
    }

    fprintf(stderr,"Test 3: Read/Write .png --->");
    if (test_imread(png) < 0) {
        fprintf(stderr," Failed\n");
    } else {
        fprintf(stderr," Success\n");
        pass++;
    }

    if (test_draw_box(jpg) < 0) {
        fprintf(stderr,"Test 4: Draw Box ---> Failed\n");
    } else {
        fprintf(stderr,"Test 4: Draw Box ---> Success\n");
        pass++;
    }

    if (test_vidread(mp4) < 0) {
        fprintf(stderr,"Test 5: Read/Write .mp4 ---> Failed\n");
    } else {
        fprintf(stderr,"Test 5: Read/Write .mp4 ---> Success\n");
        pass++;
    }

    fprintf(stderr,"Tests complete, %d/%d passed\n",pass,NTEST);
    return 0;
}
