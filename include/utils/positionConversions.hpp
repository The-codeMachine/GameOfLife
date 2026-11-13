#pragma once

#include <cstdint>
#include <utility>

/**
 * Converts a 2D position (x, y) to a 1D position (x).
 * 
 * @return the 1D position derived from a 2D position.
 */
uint32_t convert2Dto1D(uint32_t x, uint32_t y);

/**
 * Converts a 1D position (x) to a 2D position (x, y).
 * 
 * @return the 2D position derived from a 1D position as a standard pair (x 
 * is first then y).
 */
std::pair<uint32_t, uint32_t> convert1Dto2D(uint32_t x);