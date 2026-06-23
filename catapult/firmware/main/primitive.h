#ifndef CATAPULT_PRIMITIVE_H
#define CATAPULT_PRIMITIVE_H

#include <stdint.h>

typedef struct RGB RGB;

struct RGB {
    unsigned red;
    unsigned green;
    unsigned blue;
};

RGB rgb_make(uint8_t r, uint8_t g, uint8_t b);

#endif // CATAPULT_PRIMITIVE_H