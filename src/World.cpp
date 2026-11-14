#include <World.hpp>

#include <utils/positionConversions.hpp>

#include <random>

#include <algorithm>
#include <iostream>

void World::fillWorld(float temperature) noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, 100);

    // because the bounds are inclusive, we have to add one
    // so xBounds = 0 and yBounds = 0 would still result in
    // 1 cell

    //                      inclusive       inclusive
    uint64_t indexMax = (m_xBounds + 1) * (m_yBounds + 1);
    m_cells.resize(indexMax);
    for (uint64_t i = 0; i < indexMax; ++i) {
        std::pair coords = convert1Dto2D(i, m_xBounds);

        //          convert to percentage                   x           y
        Cell cell = {distrib(gen) <= temperature * 100, coords.first, coords.second};
        m_cells[i] = cell;
    }
    
}

std::vector<Cell> World::getNeighbours(Cell cell) const noexcept {
    uint32_t x = std::max(0, (int)cell.x - 1);
    uint32_t xMax = std::min(m_xBounds, cell.x + 1);

    uint32_t y = std::max(0, (int)cell.y - 1);
    uint32_t yMax = std::min(m_yBounds, cell.y + 1);
    
    std::vector<Cell> out((xMax - x) * (yMax - y));

    for (uint32_t i = x; i < xMax; ++i) {
        for (uint32_t j = y; j < yMax; ++j) {
            out.push_back(m_cells[convert2Dto1D(i, j, m_xBounds)]);
        }
    }

    return out;
}

// Needs optimization. Currently, because we call getNeighbours we
// have to iterate over neighbours twice. I don't know how to cleanly
// solve this, but it is worth a shot. 
//
//                           |   |
//                           |   |
//                           |   |
//                        ---+   +---
//                        \        /
//                         \      /
//                          \    /
//                           \  /
//                            \/

bool World::isCellAlive(Cell cell) const noexcept {
    std::vector<Cell> neighbours = getNeighbours(cell);

    uint8_t aliveCounter = 0;
    for (const Cell cell : neighbours) {
        if (cell.alive) {
            aliveCounter++;
        }
    }

    // if there are 2-3 alive cells, then it survives
    return !(aliveCounter < 2 || aliveCounter > 3);
}

// Needs optimization. Currently, we loop through all cells like 3
// times per cell. I do not know how to cleanly optimize it, but it
// is worth a shot. Maybe we add the score of each cell (e.g. 1 for 
// alive and 0 for dead) to each of it's neighbours. Based off this
// score we check whether or not it is dead. 
//
//                           |   |
//                           |   |
//                           |   |
//                        ---+   +---
//                        \        /
//                         \      /
//                          \    /
//                           \  /
//                            \/

void World::simulateTick() {
    std::vector<Cell> cells = m_cells;
    uint64_t indexMax = (m_xBounds + 1) * (m_yBounds + 1);

    for (uint64_t i = 0; i < indexMax; ++i) {
        cells[i].alive = isCellAlive(m_cells[i]);
    }

    m_cells = cells;
}

World::World(uint32_t xBounds, uint32_t yBounds, float temperature) :
            m_xBounds(xBounds), m_yBounds(yBounds) 
{
    fillWorld(temperature);
}

void World::run(int ticks) {
    // print initial status
    std::cout << "Start State: \n";
    std::cout << toString() << "\n";

    uint32_t i = 0;
    while (i < ticks || ticks == -1) {
        // simulate first tic
        simulateTick();

        // print current status
        std::cout << toString() << "\n";
        i++;
    }
}

std::string World::toString() const noexcept {
    std::string result;
    
    // xBounds and yBounds are inclusive
    for (uint32_t y = 0; y <= m_yBounds; ++y) {
        for (uint32_t x = 0; x <= m_xBounds; ++x) {
            const Cell& cell = m_cells[convert2Dto1D(x, y, m_xBounds)];
            //                  alive   dead
            result += cell.alive ? "." : " "; 
        }

        result += "\n";
    }

    result += "==============================================================";
    
    return result;
}