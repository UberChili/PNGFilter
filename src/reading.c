#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunk.h"
#include "png.h"

/* const uint8_t png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10}; */

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t colour_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
} IHDR_Info;

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
    // TODO Need to gather all IDAT chunks!
    CHUNK *chunk;
    Chunks chunks = {0};
    IHDR_Info *ihdr_bak;
    while (true) {
        chunk = read_chunk(inptr);
        if (chunk == NULL) {
            printf("Error reading Chunk or reached unexpected end of file.\n");
            break;
        }

        printf("Chunk Type: %.4s\n", chunk->chunk_type.type_code);

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
            // Copying
            /* printf("Found IHDR... Backing up...\n"); */
            ihdr_bak = (IHDR_Info *)chunk->data;
        }

        if (strncmp(chunk->chunk_type.type_code, "IDAT", 4) == 0) {
            da_append(chunks, chunk);
        } else {
            free(chunk->data);
            free(chunk);
        }

        // Free chunk after using it
        // This is the old way, so I'll comment it out for now
        // Now I need to SELECTIVELY free chunks I don't need
        /* free(chunk->data); */
        /* free(chunk); */
    }

    // TODO Try to actually use that da of IDAT chunks
    // Concatenate and decompress IDAT
    size_t lengths_sum = 0;
    for (size_t i = 0; i < chunks.count; ++i) {
        lengths_sum += chunks.items[i]->length;
    }
    uint8_t *compressed_data = malloc(lengths_sum);
    if (compressed_data == NULL) {
        printf("Error allocating data for compressed data\n");
        return 1;
    }

    size_t offset = 0;
    for (size_t i = 0; i < chunks.count; i++) {
        memcpy(compressed_data + offset, chunks.items[i]->data,
               chunks.items[i]->length);
        offset += chunks.items[i]->length;
    }

    // Free stuff and close file
    for (size_t i = 0, n = chunks.count; i < n; ++i) {
        free(chunks.items[i]->data);
        free(chunks.items[i]);
    }
    free(chunks.items);
    fclose(inptr);
    return 0;
}
