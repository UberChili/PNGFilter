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
        return 1;
    } else {
        printf("Valid PNG file.\n");
    }

    // Read the first next chunk
    CHUNK *chunk = read_chunk(inptr);
    if (chunk == NULL) {
        printf("Error reading Chunk. Exiting.\n");
        fclose(inptr);
        return 1;
    } else {
        printf("Chunk Type: %.4s\n", chunk->chunk_type.type_code);
    }

    // Free shit and close file
    free(chunk->data);
    free(chunk);
    fclose(inptr);
    return 0;
}
