#include <stdint.h>

#include "chunk_type.h"

typedef struct {
    uint32_t length;
    CHUNK_TYPE chunk_type;
    /* uint8_t type[4]; */
    uint8_t data[4];
    uint8_t crc[4];
} __attribute__((__packed__)) CHUNK;

// Try to "populate" a chunk?
/* CHUNK get_chunk() { */
/*     uint32_t length = 0; */
/*     CHUNK_TYPE chunk_t; */
/* } */
