/*
 * pnmhide/src/hide.c --
 */

#include "image.h"
#include "hide.h"
#include <string.h>

#define true            1
#define false           0
#define BITMASK         3  // Used for extracting 2 bits
#define SHIFT_FACTOR    6  // Used as a default value for a shift factor
#define SHIFT_DEC       2  // Used as a default value for shift factor decrease

typedef int bool;

/**
 * @brief Sets the "shift" least significant bits of "x" to "y".
 * 
 * @param x 
 * @param y 
 * @param shift 
 * @return The result of the operation.
 */
int set_bits(int x, int y, int shift) {
    x = (x >> shift);
    x = (x << shift);
    x = (x | y);
    return x;
}

/**
 * @brief Signs the image, indicating that there is a hidden text in it.
 * 
 * @param im - image.
 * @param dx - list of x coordinates for the signature zone.
 * @param dy - list of y coordinates for the signature zone. 
 */
void im_sign(image_t *im, int *dx, int *dy) {

    int r, g, b;

    for(int i = 0; i < 12; i += 4) {
        for(int j = i; j < i+4; j++) {
            r = PPM_GETR(im->xels[dx[j]][dy[j]]);
            g = PPM_GETG(im->xels[dx[j]][dy[j]]);
            b = PPM_GETB(im->xels[dx[j]][dy[j]]);
            switch (i/4) {
                case 0:
                    if(j%2 == 0) {
                        r = set_bits(r, BITMASK, SHIFT_DEC);
                    } else {
                        r = set_bits(r, 0, SHIFT_DEC);
                    }
                break;
                case 1:
                    if(j%2 == 0) {
                        g = set_bits(g, BITMASK, SHIFT_DEC);
                    } else {
                        g = set_bits(g, 0, SHIFT_DEC);
                    }
                break;
                default:
                    if(j%2 == 0) {
                        b = set_bits(b, BITMASK, SHIFT_DEC);
                    } else {
                        b = set_bits(b, 0, SHIFT_DEC);
                    }
                break;
            }

            PPM_ASSIGN(im->xels[dx[j]][dy[j]], r, g, b);
        }
    }
}

/**
 * @brief Verify that the image is signed, i.e. there is a hidden text embeded in it.
 * 
 * @param im - image
 * @param dx - list of x coordinates for the signature zone.
 * @param dy - list of y coordinates for the signature zone.
 * @return True if image is signed, false otherwise.
 */
bool im_verify(image_t *im, int *dx, int *dy) {
    
    int r, g, b;

    for(int i = 0; i < 12; i += 4) {
        for(int j = i; j < i+4; j++) {
            r = PPM_GETR(im->xels[dx[j]][dy[j]]);
            g = PPM_GETG(im->xels[dx[j]][dy[j]]);
            b = PPM_GETB(im->xels[dx[j]][dy[j]]);
            switch (i/4) {
                case 0:
                    if(j%2 == 0) {
                        if((r & BITMASK) != BITMASK) return false;
                    } else {
                        if((r & BITMASK) != 0) return false;
                    }
                break;
                case 1:
                    if(j%2 == 0) {
                        if((g & BITMASK) != BITMASK) return false;
                    } else {
                        if((g & BITMASK) != 0) return false;
                    }
                break;
                default:
                    if(j%2 == 0) {
                        if((b & BITMASK) != BITMASK) return false;
                    } else {
                        if((b & BITMASK) != 0) return false;
                    }
                break;
            }
        }
    }
    return true;
}

/**
 * @brief Check whether the current indicies are in the signature zone. 
 * 
 * @param x - x coordinate or i-index.
 * @param y - y coordinate or j-index.
 * @param dx - list of x coordinates for the signature zone.
 * @param dy - list of y coordinates for the signature zone.
 * @return True if in signature zone, false otherwise.
 */
bool check(int x, int y, int *dx, int *dy) {
    for(int i = 0; i < 12; i++) {
        if(x == dx[i] && y == dy[i]) return true;
    }
    return false;
}

int im_encode(image_t *im, char *msg) {

    /* Establish signature zone */
    int n = im->rows;
    int m = im->cols;
    int dx[] = {0, 1, 2, 3, n-1, n-1, n-1, n-1, n-1, n-2, n-3, n-4};
    int dy[] = {m-1, m-1, m-1, m-1, m-1, m-2, m-3, m-4, 0, 0, 0, 0};
    im_sign(im, dx, dy);
    /* ---------- */    

    int len = strlen(msg);  // Length of the message
    int msg_i = 0;  // The index pointing to a character in msg
    int lsb_i = SHIFT_FACTOR;  // LSB shift factor
    /* The maximum length of hidden text the image can encode, 12 pixels are used for signature. */
    int max_len = (im->rows*im->cols*6)/8 - 12;
    int cnt = 0;
    bool done = false;

    u_int8_t r, g, b;
    
    if(len > max_len) {
        fprintf(stderr, "Error: image not big enough\n");
        return -1;
    }

    for(int i = 0; i < n && done == 0; i++) {
        for(int j = 0; j < m && done == 0; j++) {
            if(check(i, j, dx, dy)) continue;
            for(int rgb_i = 0; rgb_i < 3; rgb_i++){

                /* Get the RGB values of the pixel */
                r = PPM_GETR(im->xels[i][j]);
                g = PPM_GETG(im->xels[i][j]);
                b = PPM_GETB(im->xels[i][j]);

                if(msg_i >= len) {

                    /* Message is already encoded */
                    switch (rgb_i) {
                        case 0:
                            r = set_bits(r, 0, SHIFT_DEC);
                        break;
                        case 1:
                            g = set_bits(g, 0, SHIFT_DEC);
                        break;
                        default:
                            b = set_bits(b, 0, SHIFT_DEC);
                        break;
                    }
                    cnt++;
                } else {

                    /* Encode 2 bits of the character in either R, G or B value */
                    char c = msg[msg_i];
                    u_int8_t tmp = (c >> lsb_i) & BITMASK;
                    switch (rgb_i) {
                        case 0:
                            r = set_bits(r, tmp, SHIFT_DEC);
                        break;
                        case 1:
                            g = set_bits(g, tmp, SHIFT_DEC);
                        break;
                        default:
                            b = set_bits(b, tmp, SHIFT_DEC);
                        break;
                    }
                }

                /* Change the encoded pixel */
                PNM_ASSIGN(im->xels[i][j], r, g, b);

                if(msg_i < len) {
                    if(lsb_i == 0) {
                        lsb_i = SHIFT_FACTOR;
                        msg_i++;
                    } else {
                        lsb_i -= SHIFT_DEC;
                    }
                }
                if(cnt == 4) {
                    done = true;
                    break;
                }
            }
        }
    }
    return len;
}

int im_decode(image_t *im, char *buffer, size_t size) {

    int n = im->rows;
    int m = im->cols;
    int dx[] = {0, 1, 2, 3, n-1, n-1, n-1, n-1, n-1, n-2, n-3, n-4};
    int dy[] = {m-1, m-1, m-1, m-1, m-1, m-2, m-3, m-4, 0, 0, 0, 0};

    int done = 0;
    int len = 0;
    char c = 0;
    int cnt = 0;

    u_int8_t r, g, b;

    if(im_verify(im, dx, dy) == false) {
        buffer[len] = '\0';
        return 0;
    }

    for(int i = 0; i < n && done == 0; i++) {
        for(int j = 0; j < m && done == 0; j++) {

            if(check(i, j, dx, dy)) continue;

            for(int rgb_i = 0; rgb_i < 3; rgb_i++) {

                /* Get the RGB values of the pixel */
                r = PPM_GETR(im->xels[i][j]);
                g = PPM_GETG(im->xels[i][j]);
                b = PPM_GETB(im->xels[i][j]);

                switch (rgb_i) {
                    case 0:
                    c = ((c << 2) | (r & BITMASK)); 
                    break;
                    case 1:
                    c = ((c << 2) | (g & BITMASK)); 
                    break;
                    default:
                    c = ((c << 2) | (b & BITMASK)); 
                    break;
                }
                cnt++;

                if(cnt == 4) {

                    cnt = 0;

                    if(c == 0) {
                        done = 1;
                        break;
                    } else {
                        buffer[len] = c;
                        len++;
                        c = 0;
                    }
                }
            }
        }

    }

    buffer[len] = '\0';
    return len;
}