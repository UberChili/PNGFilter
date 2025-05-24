#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "png.h"

/* const uint8_t png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10}; */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./reading <png file>\n");
    }

    // Open input file
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL) {
        printf("Could not open %s\n", infile);
        return 1;
    }

    // Read infile's PNGSIGNATUREHEADER and determine if
    // it is a valid PNG file
    PNGSIGNATUREHEADER sh;
    if (!valid_SH(&sh, inptr)) {
        printf("Not a valid PNG File.\n");
    } else {
        printf("Valid PNG file.\n");
    }

    // Read the first next chunk
    uint32_t length;
    if (fread(&length, sizeof(uint32_t), 1, inptr) != 1) {
        printf("Error: Couldn't read bytes for length\n");
        return 1;
    }
    CHUNK_TYPE chunk_type;
    if (fread(&chunk_type, sizeof(chunk_type.type_code), 1, inptr) != 1) {
        printf("Error: Couldn't read bytes for chunk type\n");
        return 1;
    }

    /* uint8_t data[length]; */
    uint8_t *data = malloc(sizeof(uint8_t) * length);
    if (data == NULL) {
        printf("Error allocating space for chunk data\n");
        free(data);
        return 1;
    }
    if (fread(data, sizeof(uint8_t), length, inptr) == length) {
        printf("Error: Couldn't read bytes for data\n");
        free(data);
        return 1;
    }

    uint8_t crc[4];
    if (fread(&crc, 1, 4, inptr) == 4) {
        printf("Error: Couldn't read bytes for crc\n");
        return 1;
    }

    // do some sort of checking here
    printf("Chunk Type: %.4s\n", chunk_type.type_code);

    // Free shit and close file
    free(data);
    fclose(inptr);
    return 0;
}
