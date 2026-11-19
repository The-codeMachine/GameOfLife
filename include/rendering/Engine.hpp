#pragma once

#include <Cell.hpp>
#include <rendering/CellRendering.hpp>
#include <World.hpp>

#include <atomic>

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
     * Stops rendering the simulation at once. It will end the
     * rendering but not the simulation.
     */
    void stop();

private:

    /**
     * Creates an FRect for a single Cell. Includes its position
     * and size data (as well as a border).
     * 
     * @param cell
     */
    static CellSprite createCellRenderingObject(Cell cell);

    /**
     * Creates a vector of FRect for all the Cells in the World this
     * Renderer engine is connected to. 
     */
    std::vector<CellSprite> createCellsRenderingObject();

    /**
     * Draws a bordered rectangle (using the FRect object) using this 
     * Engine's renderer. Fills the interior with a certain colour and
     * the extior (border) with another. Border thickness is always 1
     * pixel. 
     * 
     * @param rect the dimensions and position of the rectangle
     * @param fillColor the color of the interior of the rectangle
     * @param borderColor the color of the border of the rectangle
     */
    void drawBorderedFRect(const SDL_FRect* rect, SDL_Color fillColor, SDL_Color borderColor);

    /**
     * Draws all of the connected World's cells. Will generate a FRect
     * for each cell, then draw each one. 
     */
    void drawCells();

private:
    World& m_world;    

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    std::atomic<bool> m_running{false};

};