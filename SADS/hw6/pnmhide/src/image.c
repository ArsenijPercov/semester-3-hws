/*
 * pnmhide/src/image.c --
 */

#define _POSIX_C_SOURCE 2

#include <string.h>
#if 0
#include <netpbm/pnm.h>
#else
#include <pnm.h>
#endif
#include "image.h"

int
im_read(image_t *im, char *file)
{
    FILE *in;
 
    in = fopen(file, "r");
    if (! in) {
        perror("fopen()");
        return 1;
    }

    im->xels = pnm_readpnm(in, &im->cols, &im->rows, &im->maxval, &im->format);
    if (! im->xels) {
        fclose(in);
        return 2;
    }
    fclose(in);
    return 0;
}

int
im_write(image_t *im, char *file)
{
    FILE *out;

    if (! file) {
        out = stdout;
    } else {
        out = fopen(file, "w");
        if (! out) {
            perror("fopen()");
            return 1;
        }
    } 

    pnm_writepnm(out, im->xels, im->cols, im->rows, im->maxval, im->format, 0);

    if (file) {
        fclose(out);
    }
    return 0;
}

void
im_free(image_t *im)
{
    if (im && im->xels) {
        pnm_freearray(im->xels, im->rows);
    }
}
