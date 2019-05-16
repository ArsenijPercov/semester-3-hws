/*
 * pnmhide/src/hide.h --
 */

#ifndef _HIDE_H
#define _HIDE_H

#include "image.h"

/**
 * \brief Encode a message in an image.
 *
 * \param im pointer to the image.
 * \param msg message to encode.
 * \result length of the message encoded or negative number on error.
 */

int
im_encode(image_t *im, char *msg);

/**
 * \brief Decode a message from an image.
 *
 * \param im pointer to the image.
 * \param buffer buffer to fill with the decoded message.
 * \result length of the message decoded or negative number on error.
 */

int
im_decode(image_t *im, char *buffer, size_t size);

#endif
