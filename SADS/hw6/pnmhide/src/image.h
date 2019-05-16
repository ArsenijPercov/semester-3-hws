/*
 * pnmhide/src/image.h --
 */

#ifndef _IMAGE_H
#define _IMAGE_H

#define _POSIX_C_SOURCE 2

#if 0
#include <netpbm/pnm.h>
#else
#include <pnm.h>
#endif

typedef struct image {
    xel **xels;         /*!< Two-dimensional array of pixels */
    xelval maxval;      /*!< The maximum value of a pixel */
    int cols;           /*!< The number of columns */
    int rows;           /*!< The number of rows */
    int format;         /*!< The external format of the image */
} image_t;

/**
 * \brief Read an image from a file. If the file name is NULL, read
 *        from the standard input.
 *
 * \param im pointer to the image.
 * \param name of the image file.
 * \result 0 on success, positive error code on failure
 */

int
im_read(image_t *im, char *file);

/**
 * \brief Write an image to a file. If the file name is NULL, write
 *        to the standard input.
 *
 * \param im pointer to the image.
 * \param name of the image file.
 * \result 0 on success, positive error code on failure
 */

int
im_write(image_t *im, char *file);

/**
 * \brief Free the memory allocated for an image.
 *
 * \param im pointer to the image.
 */

void
im_free(image_t *im);

#endif
