#include "frameio.h"
#include "imtools.h"

int main(int argc, char **argv)
{
    (void) argc;
    float scale;
    const char *filename = argv[1];
    const char *imscale = argv[2];
    const char *alg = argv[3];
    sscanf(imscale,"%f",&scale);
    
    rgb img;
    rgb resized;
    fio_imread(filename,&img,-1,-1);
    imresize(&img, &resized, scale*img.h, scale*img.w, alg); 
    fio_imwrite("output.jpg",&resized);

    free(img.data);
    free(resized.data);
}
