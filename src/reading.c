#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunk.h"
#include "png.h"

/* const uint8_t png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10}; */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./reading <png file>\n");
        return 1;
    }

    // Open input file
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "rb+");
    if (inptr == NULL) {
        printf("Could not open %s\n", infile);
        return 1;
    }

    // Read infile's PNGSIGNATUREHEADER and determine if
    // it is a valid PNG file
    PNGSIGNATUREHEADER sh;
    if (!valid_SH(&sh, inptr)) {
        printf("Not a valid PNG File.\n");
        return 1;
    } else {
        printf("Valid PNG file.\n");
    }

    // Start reading Chunks to the End of File! (I guess)
    CHUNK *chunk;
    while (true) {
        chunk = read_chunk(inptr);
        if (chunk == NULL) {
            printf("Error reading Chunk or reached unexpected end of file.\n");
            break;
        }

        printf("Chunk Type: %.4s\n", chunk->chunk_type.type_code);
        // TODO In here we should do some kind of modification, maybe
        // Try to do it in a function

        // Check if we reached the end of the PNG file
        // (Last chunk is always IEND)
        if (strncmp(chunk->chunk_type.type_code, "IEND", 4) == 0) {
            printf("Reached end of PNG file.\n");
            free(chunk->data);
            free(chunk);
            break;
        }

        // Get some information
        if (strncmp(chunk->chunk_type.type_code, "IHDR", 4) == 0) {
            printf("Found IHDR... Getting some info:\n");
            print_image_info(chunk);
        }

        // Free chunk after using it
        free(chunk->data);
        free(chunk);
    }

    // Free stuff and close file
    fclose(inptr);
    return 0;
}
