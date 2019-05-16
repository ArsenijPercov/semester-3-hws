/*
 * pnmhide/src/pnmhide.c --
 */

#define _POSIX_C_SOURCE 2

#include <unistd.h>
#if 0
#include <netpbm/pnm.h>
#else
#include <pnm.h>
#endif
#include "image.h"
#include "hide.h"

static const char *progname = "pnmhide";

int main(int argc, char *argv[])
{
    image_t im;
    char opt;
    char *msg = NULL;
    int eflag = 0, dflag = 0;
    char buffer[4096];

    while ((opt = (char) getopt(argc, argv, "e:d")) != -1) {
        switch (opt) {
        case 'e':
            eflag = 1;
            msg = optarg;
            break;
        case 'd':
            dflag = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-e msg] [-d] pnmfile\n", progname);
            exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected file name after options\n");
        exit(EXIT_FAILURE);
    }
    
    pm_init(progname, 0);

    if (im_read(&im, argv[optind]) != 0) {
        return EXIT_FAILURE;
    }

    if (eflag && msg) {
        im_encode(&im, msg);
        if (im_write(&im, NULL) != 0) {
            im_free(&im);
            return EXIT_FAILURE;
        }
    }

    if (dflag) {
        int len = im_decode(&im, buffer, sizeof(buffer));
        printf("%s", buffer);
        if(len != 0) printf("\n");
    }
    
    im_free(&im);
    return EXIT_SUCCESS;
}
