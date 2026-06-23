#include "primitive.h"

RGB rgb_make(uint8_t r, uint8_t g, uint8_t b){
    RGB _ = {
        .red = r,
        .green = g,
        .blue = b
    };
    return _;
}