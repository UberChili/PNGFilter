#include <stdint.h>
#include <stdio.h>

#include "chunk_type.h"

typedef struct {
    uint32_t length;
    CHUNK_TYPE chunk_type;
    uint8_t *data;
    uint32_t crc;
} __attribute__((__packed__)) CHUNK;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t colour_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
} __attribute__((__packed__)) IHRD_CHUNK;

// Helper to swap endianness
uint32_t swap_endian(uint32_t val);

// Reads a chunk from file
CHUNK* read_chunk(FILE *file);

// Prints information from IHDR Chunk
void interpret_IHDR(CHUNK *chunk);
