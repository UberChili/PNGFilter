#ifndef PNG_H
#define PNG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Struct for the Signature Header
typedef struct {
    uint8_t values[8];
} __attribute__((__packed__)) PNGSIGNATUREHEADER;

// Checks for a valid Signature Header
bool valid_SH(PNGSIGNATUREHEADER *signature_header, FILE *file);

#endif
