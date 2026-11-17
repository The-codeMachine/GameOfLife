#pragma once

#include <SDL3/SDL.h>

#include <Cell.hpp>
#include <World.hpp>

class Engine {
public:

    Engine(World& world, const uint32_t windowX = 400, const uint32_t windowY = 400);

    ~Engine();

    /**
     * Renders the world with as much FPS as possible. Renders
     * all cells within 1 window. Will open a window. 
     */
    void render();

    /**
     * Stops rendering the simulation at once. It will kill the
     * rendering but not the simulation.
     */
    void stop();

private:

    /**
     * 
     * 
     * @param cell
     */
    static void createCellRenderingObject(Cell cell);

    /**
     * 
     */
    void createRenderingObject();

private:
    World& world;    

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

};