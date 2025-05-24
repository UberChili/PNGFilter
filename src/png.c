#include "png.h"

// PNG Signature. It is always the following values
const uint8_t png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10};

bool valid_SH(PNGSIGNATUREHEADER *signature_header, FILE *file) {
    if (fread(signature_header, sizeof(PNGSIGNATUREHEADER), 1, file) != 1) {
        printf("Error: Couldn't read Signature Header.\n");
        return false;
    }

    // Compare each element in the Signature Header
    for (size_t i = 0, n = sizeof(signature_header->values) / sizeof(uint8_t);
         i < n; i++) {
        /* printf("%02x ", signature_header->values[i]); */
        printf("%u ", signature_header->values[i]);
        if (signature_header->values[i] != png_signature[i]) {
            printf("Mismatch in Signature Header.\n");
            return false;
        }
    }

    return true;
}
