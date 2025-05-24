#include <stdint.h>
#include <stdio.h>

#include "chunk_type.h"

typedef struct {
    uint32_t length;
    CHUNK_TYPE chunk_type;
    uint8_t *data;
    uint32_t crc;
} __attribute__((__packed__)) CHUNK;

// Helper to swap endianness
uint32_t swap_endian(uint32_t val);

// Reads a chunk from file
CHUNK* read_chunk(FILE *file);
