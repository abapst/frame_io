#include "frameio.h"
#include "imtools.h"

int main(int argc, char **argv)
{
    float scale;
    const char *filename = argv[1];
    const char *imscale = argv[2];
    const char *alg = argv[3];
    rgb img;
    rgb resized;

    (void) argc;
    if (argc != 4) {
        fprintf(stderr, "Usage: ./imresize <filename> <scale_factor> <algorithm = {'nearest','bilinear'}>\n");
        return 0;
    }

    sscanf(imscale,"%f",&scale);

    fio_imread(filename,&img,-1,-1);
    imresize(&img, &resized, scale*img.h, scale*img.w, alg);
    fio_imwrite("output.png",&resized);

    free(img.data);
    free(resized.data);

    return 0;
}
