#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// on Linux
#include <arpa/inet.h>

#include "chunk.h"

const uint32_t MAX_CHUNK_LENGTH = 2147483647;

uint32_t swap_endian(uint32_t val) {
    return ((val >> 24) & 0xff) | ((val >> 8) & 0xff00) |
           ((val << 8) & 0xff0000) | ((val << 24) & 0xff000000);
}

CHUNK *read_chunk(FILE *fileptr) {
    if (fileptr == NULL) {
        printf("Error with file.\n");
        return NULL;
    }

    // Allocating memory for chunk
    CHUNK *chunk = malloc(sizeof(CHUNK));
    if (chunk == NULL) {
        printf("Error allocating memory for Chunk.\n");
        return NULL;
    }

    // Read length
    if (fread(&chunk->length, sizeof(uint32_t), 1, fileptr) != 1) {
        printf("Error: Couldn't read bytes for Length.\n");
        free(chunk);
        return NULL;
    }
    chunk->length = swap_endian(chunk->length);

    // Read Chunk Type
    if (fread(&chunk->chunk_type, sizeof(chunk->chunk_type), 1, fileptr) != 1) {
        printf("Error: Couldn't read bytes for Chunk Type.\n");
        free(chunk);
        return NULL;
    }

    // Allocate and Read Data
    chunk->data = malloc(sizeof(uint8_t) * chunk->length);
    if (chunk->data == NULL) {
        printf("Error: Couldn't allocate memory for Chunk Data.\n");
        return NULL;
    }
    if (fread(chunk->data, sizeof(uint8_t), chunk->length, fileptr) !=
        chunk->length) {
        printf("Error: Couldn't read bytes for Chunk Data.\n");
        free(chunk->data);
        free(chunk);
        return NULL;
    }

    // Read CRC into temporary buffer, then copy
    uint8_t temp_crc[4];
    if (fread(temp_crc, 1, 4, fileptr) != 4) {
        printf("Error: Couldn't read CRC.\n");
        free(chunk->data);
        free(chunk);
        return NULL;
    }
    memcpy(&chunk->crc, temp_crc, 4);

    return chunk;
}

// Try to retrieve information from IHDR Chunk
// Previously named "interpret_IHDR"
void print_image_info(CHUNK *chunk) {
    printf("Length field contained in Chunk: %d\n", chunk->length);
    IHRD_CHUNK *ihdr = (IHRD_CHUNK *)chunk->data;
    printf("Width: %u\n", ntohl(ihdr->width));
    printf("Height: %u\n", ntohl(ihdr->height));
    printf("Bit depth: %u\n", ihdr->bit_depth);
    printf("Colour type: %u\n", ihdr->colour_type);
    printf("Compression method: %u\n", ihdr->compression_method);
    printf("Filter method: %u\n", ihdr->filter_method);
    printf("Interlace method: %u\n", ihdr->interlace_method);
}
