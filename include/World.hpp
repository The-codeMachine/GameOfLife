#pragma once

#include <Cell.hpp>

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
     * "-" border
     * 
     * @return a string representing the World's current state. 
     */
    std::string toString() const noexcept;

private: 
    /**
     * Checks whether or not a cell will be alive during the next tick of the
     * simulation.
     * 
     * @return true if the cell will be alive during the next tick and false otherwise
     */
    bool isCellAlive() const noexcept;

    /**
     * Gets copies neighbours of a particular cell. 
     * 
     * @note does not include invalid cell positions (e.g. -1, -1).
     * @note the neighbours are stored in order (from {-1, -1} to {1, 1} -- based off the 
     * cell's position)
     * 
     * @return a vector containing copies of all the neighbours of a particular cell
     */
    std::vector<Cell> getNeighbours() noexcept;

    /**
     * Simulates one tick. Updates all appropriate values (m_cells).
     */
    void simulateTick();

private:
    const uint32_t xBounds;
    const uint32_t yBounds;

    // 1D representation of cells
    std::vector<uint32_t, Cell> m_cells;

};