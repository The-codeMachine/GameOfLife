#include <positionConversions.hpp>

uint32_t convert2Dto1D(uint32_t x, uint32_t y, uint32_t xBounds) {
    return x + y * xBounds;
}

std::pair<uint32_t, uint32_t> convert1Dto2D(uint32_t x, uint32_t xBounds) {
    //          x           y
    return {x % xBounds, x / xBounds};
}