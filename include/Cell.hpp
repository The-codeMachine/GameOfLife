#pragma once

#include <cstdint>

/**
 * A single cell. Contains data relating to whether or not the
 * cell is alive, and where it is located in the World from the
 * origin. 
 * 
 */
struct Cell {
    // true if the cell is alive and false otherwise
    bool alive; 

    uint32_t x; // the x value from the origin (always positive)
    uint32_t y; // the y value from the origin (always positive)
};