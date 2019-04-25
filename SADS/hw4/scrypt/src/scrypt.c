//
// Created by Dushan Terzikj on 25.04.19.
//
#include "scrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


/**
 * @brief The inverse of the modulo function used in the S-boxes.
 */
static uint8_t s_box_inv[] = {15, 6, 13, 4, 11, 2, 9, 0, 7, 14, 5, 12, 3, 10, 1, 8};

/**
 * @brief Returns the 4 most significant bits of an 8-bit number.
 * @param x 8-bit cipher text.
 * @return 4 most significant bits of an 8-bit number.
 */
uint8_t get_left(uint8_t x) {
    return (uint8_t) ((x >> 4) & ((1 << 4) - 1));
}

/**
 * @brief Returns the 4 least significant bits of an 8-bit number.
 * @param x 8-bit cipher text.
 * @return 4 least significant bits of an 8-bit number.
 */
uint8_t get_right(uint8_t x) {
    return (uint8_t) (x & ((1 << 4) - 1));
}

/**
 * @brief Subsitution function used in the encryption.
 * @return 8-bit cipher text.
 */
uint8_t sub(uint8_t x) {
    return (uint8_t) (((x + 1)*7)%16);
}

/**
 * @brief Permutation function used in encryption.
 * @param x 8-bit cipher text.
 * @return 8-bit cipher text.
 */
uint8_t permutation(uint8_t x) {
    uint8_t res = x;
    uint8_t msb2 = (uint8_t) ((x >> 6)&((1 << 2) - 1));
    res = (res << 2);
    res = (res | msb2);
    return res;
}

/**
 * @brief Inverse permutation function used in encryption.
 * @param x 8-bit cipher text
 * @return 8-bit cipher text.
 */
uint8_t permutation_inv(uint8_t x) {
    uint8_t res = x;
    uint8_t lsb2 = (uint8_t) (x&((1 << 2) - 1));
    res = (res >> 2);
    res = (res | (lsb2 << 6));
    return res;
}

uint8_t sc_enc8(uint8_t m, uint32_t k) {


    uint8_t keys[4];  // The 32-bit key broken into 4 8-bit keys
    uint8_t c = m;  // The cipher text which will be the result at the end.
    uint8_t left, right; // Helper variables used later
    int i;

    /* Acquire 4 8-bit keys from the 32-bit key */
    for(i = 0; i < 4; i++) {
        keys[i] = (uint8_t) ((k >> ((4-i-1)*8)) & ((1 << 8) - 1));
    }


    i = 0;
    /* Round 0 */
    c = c^keys[i++];

    /* Round 1 and 2 */
    for(;i < 3; i++) {
        left = get_left(c);
        right = get_right(c);
        left = sub(left);
        right = sub(right);
        c = ((left << 4) | right);
        c = permutation(c);
        c = c ^ keys[i];
    }

    /* Round 3 */
    left = get_left(c);
    right = get_right(c);
    left = sub(left);
    right = sub(right);
    c = ((left << 4) | right);
    c = c ^ keys[i];

    return c;
}

uint8_t sc_dec8(uint8_t c, uint32_t k) {

    uint8_t keys[4];  // The 32-bit key broken into 4 8-bit keys
    uint8_t m = c;  // The clear text which will be the result at the end.
    uint8_t left, right; // Helper variables used later
    int i;

    /* Acquire 4 8-bit keys from the 32-bit key */
    for(i = 0; i < 4; i++) {
        keys[i] = (uint8_t) ((k >> (i*8)) & ((1 << 8) - 1));
    }

    i = 0;
    /* Inverse Round 3 */
    m = m^keys[i++];
    left = get_left(m);
    right = get_right(m);
    left = s_box_inv[left];
    right = s_box_inv[right];
    m = ((left << 4) | right);

    /* Inverse Round 1 and 2 */
    for(; i < 3; i++) {
        m = m^keys[i];
        m = permutation_inv(m);
        left = get_left(m);
        right = get_right(m);
        left = s_box_inv[left];
        right = s_box_inv[right];
        m = ((left << 4) | right);
    }

    m = m^keys[i];
    return m;
}

void sc_enc_ecb(unsigned char *m, unsigned char *c, size_t len, uint32_t k) {

    int i;

    for(i = 0; i < len; i++) {
        c[i] = sc_enc8(m[i], k);
    }
}

void sc_dec_ecb(unsigned char *c, unsigned char *m, size_t len, uint32_t k) {

    int i;

    for(i = 0; i < len; i++) {
        m[i] = sc_dec8(c[i], k);
    }
}

void sc_enc_cbc(unsigned char *m, unsigned char *c, size_t len, uint32_t k, uint8_t iv) {

    int i = 0;
    uint8_t blocks[len];
    for(i = 0; i < len; i++) {
        blocks[i] = m[i];
    }

    for(i = 0; i < len; i++) {
        blocks[i] = (blocks[i]^iv);
        c[i] = sc_enc8(blocks[i], k);
        iv = c[i];
    }
}

void sc_dec_cbc(unsigned char *c, unsigned char *m, size_t len, uint32_t k, uint8_t iv) {

    int i;

    for(i = 0; i < len; i++) {
        m[i] = sc_dec8(c[i], k);
        m[i] = m[i]^iv;
        iv = c[i];
    }
}
