#pragma once

#include <Cell.hpp>

#include <string>
#include <vector>

#include <optional>
#include <chrono>

/**
 * The World class contains all information about the simulation including all
 * the cells, their positional data, and alive/death information. 
 */
class World {
public:
    /**
     * Constructs the World with specific x and y bounds. The temperature defines
     * what the chances of a cell being generated per square is. 
     * 
     * bounds (xBounds and yBounds) are inclusive to 0
     * 
     * @note temperature is used as a percentage (e.g. 0.7 = 70%)
     */
    World(uint32_t xBounds, uint32_t yBounds, float temperature = 0.7);

    /**
     * Gets a copy of all the cells in this World
     * 
     * @return a vector consists of all the Cells in this World
     */
    std::vector<Cell> cells();

    /**
     * Gets all the cells in this world and returns a read-only copy.
     * 
     * @return a read-only reference to a vector consisting of all the cells in this
     * World
     */
    const std::vector<Cell>& cells() const noexcept;

    /**
     * Runs the World's simulation. Makes a string representation of the World
     * and prints that at the end of every tick. 
     * 
     * @param ticks represents the amount of ticks the simulation should move 
     * forward before terminating
     * 
     * @param pauseTime represents the amount of time the program will wait 
     * between ticks. This is for infinite simulations simply so we can 
     * observe and see. 
     * 
     * @note if ticks == -1 then it will never end.  
     */
    void run(int ticks = -1, std::optional<std::chrono::milliseconds> pauseTime = std::chrono::milliseconds(1000));

    /**
     * Makes a string representation of the World. Will contain
     * basic characters for cells.
     * 
     * " " - dead cell
     * "." - alive cell
     * 
     * @return a string representing the World's current state. 
     */
    std::string toString() const noexcept;

private: 
    /**
     * Fills the entire world. Bases how many cells exist off the temperature. For
     * example if the temperature == 0.7 then each cell has a 70% chance of being
     * alive. 
     * 
     * @note uses the same seed for all Worlds (for now, might change it)
     * @note temperature is used as a percentage (e.g. 0.7 = 70%)
     */
    void fillWorld(float temperature);

    /**
     * Checks whether or not a cell will be alive during the next tick of the
     * simulation.
     * 
     * @return true if the cell will be alive during the next tick and false otherwise
     */
    bool isCellAlive(Cell cell) const noexcept;

    /**
     * Gets a particular cell's neighbour's alive count. Checks every neighbour to see
     * whether or not they are alive.
     * 
     * @return a number 0-8 (inclusive) that represents the number of alive neighbours
     */
    uint16_t getNeighbours(Cell cell) const noexcept;

    /**
     * Simulates one tick. Updates all appropriate values (m_cells).
     */
    void simulateTick();

private:
    const uint32_t m_xBounds;
    const uint32_t m_yBounds;

    // 1D representation of cells
    std::vector<Cell> m_cells;

};