//
// Created by Dushan Terzikj on 25.04.19.
//
#include "scrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {

    // a)
    fprintf(stderr, "\na)\n\nCheck scrypt.c");
    fprintf(stderr, "\n\n----------\n\nb)\n\n");

    // b)
    uint32_t k = 0x98267351;
    uint8_t iv = 0x42;

    unsigned char *clear = (unsigned char*) "secret";
    size_t len = strlen((char *) clear);
    unsigned char b[len];
    fprintf(stderr, "Encrypting clear text: %s\n", clear);
    sc_enc_ecb(clear, b, len, k);
    fprintf(stderr, "Encrypted %s = 0x", clear);
    for(int i = 0; i < len; i++) fprintf(stderr, "%x", b[i]);
    fprintf(stderr, "\n\n----------\n\nc)\n\n");

    // c)
    clear = (unsigned char*) "hacker";
    len = strlen((char *) clear);
    unsigned char d[len];
    fprintf(stderr, "\nEncrypting clear text: %s\n", clear);
    sc_enc_cbc(clear, d, len, k, iv);
    fprintf(stderr, "Encrypted %s = 0x", clear);
    for(int i = 0; i < len; i++) fprintf(stderr, "%x", d[i]);
    fprintf(stderr, "\n\n----------\n\nd)\n\n");

    // d)
    unsigned char cipher[] = {0xc6, 0x5e, 0x05, 0x94, 0x6b, 0x86, 0xeb, 0x2e, 0x33, 0xf5, 0x8f, 0xda, 0xff, 0x0f, 0x42};
    len = strlen((char *) cipher);
    unsigned char e[len];
    fprintf(stderr, "\nDecrypting cipher text: 0x");
    for(int i = 0; i < len; i++) fprintf(stderr, "%x", cipher[i]);
    sc_dec_cbc(cipher, e, len, k, iv);
    fprintf(stderr, "\nDecrypted 0x");
    for(int i = 0; i < len; i++) fprintf(stderr, "%x", cipher[i]);
    fprintf(stderr, " = %s\n\n", e);

    return EXIT_SUCCESS;
}