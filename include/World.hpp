#pragma once

#include <Cell.hpp>

#include <string>
#include <vector>

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
     * Runs the World's simulation. Makes a string representation of the World
     * and prints that at the end of every tick. 
     * 
     * @param ticks represents the amount of ticks the simulation should move 
     * forward before terminating
     * 
     * @note if ticks == -1 then it will never end.  
     */
    void run(int ticks = -1);

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
    void fillWorld(float temperature) noexcept;

    /**
     * Checks whether or not a cell will be alive during the next tick of the
     * simulation.
     * 
     * @return true if the cell will be alive during the next tick and false otherwise
     */
    bool isCellAlive(Cell cell) const noexcept;

    /**
     * Gets copies neighbours of a particular cell. 
     * 
     * @note does not include invalid cell positions (e.g. -1, -1).
     * @note the neighbours are stored in order (from {-1, -1} to {1, 1} -- based off the 
     * cell's position)
     * 
     * @return a vector containing copies of all the neighbours of a particular cell
     */
    std::vector<Cell> getNeighbours(Cell cell) const noexcept;

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