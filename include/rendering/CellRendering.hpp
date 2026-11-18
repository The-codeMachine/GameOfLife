#pragma once

#include <SDL3/SDL.h>

/**
 * The cell rendering object. This determines things like
 * it's dimensions and location (through an SDL_FRect object)
 * and its alive/dead status through a boolean.  
 */
struct CellSprite {
    SDL_FRect frect;
    bool alive;
};