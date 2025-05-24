#include <ctype.h>

typedef struct {
    /* uint8_t type_code[4]; */
    char type_code[4];
} __attribute__((__packed__)) CHUNK_TYPE;

/* int is_valid(CHUNK_TYPE chunk_type) { return 0; } */

int is_critical(CHUNK_TYPE chunk_type) {
    if (sizeof chunk_type.type_code != 4) {
        return 0;
    }
    if (isupper(chunk_type.type_code[0])) {
        return 1;
    }
    return 0;
}

int is_public(CHUNK_TYPE chunk_type) {
    if (sizeof chunk_type.type_code != 4) {
        return 0;
    }
    if (isupper(chunk_type.type_code[1])) {
        return 1;
    }
    return 0;
}

int is_reserved(CHUNK_TYPE chunk_type) {
    if (sizeof chunk_type.type_code != 4) {
        return 0;
    }
    if (isupper(chunk_type.type_code[2])) {
        return 1;
    }
    return 0;
}

int is_safe_to_copy(CHUNK_TYPE chunk_type) {
    if (sizeof chunk_type.type_code != 4) {
        return 0;
    }
    if (isupper(chunk_type.type_code[3])) {
        return 0;
    }
    return 1;
}
