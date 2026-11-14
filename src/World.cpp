#include <World.hpp>

#include <positionConversions.hpp>

#include <algorithm>
#include <iostream>

std::vector<Cell> World::getNeighbours(Cell cell) const noexcept {
    uint32_t x = std::max(0, (int)cell.x - 1);
    uint32_t xMax = std::min(xBounds, cell.x + 1);

    uint32_t y = std::max(0, (int)cell.y - 1);
    uint32_t yMax = std::min(yBounds, cell.y + 1);
    
    std::vector<Cell> out((xMax - x) * (yMax - y));

    for (uint32_t i = x; i < xMax; ++i) {
        for (uint32_t j = y; j < yMax; ++j) {
            out.push_back(m_cells[convert2Dto1D(i, j, xBounds)]);
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
// is worth a shot. 
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

    for (uint32_t x = 0; x < xBounds; x++) {
        for (uint32_t y = 0; y < yBounds; y++) {
            uint32_t index = convert2Dto1D(x, y, xBounds);

            cells[index].alive = isCellAlive(m_cells[index]);
        }
    }

    m_cells = cells;
}

void World::run(int ticks) {
    // print initial status
    std::cout << toString() << "\n";
    
    uint32_t i = 0;
    while (i < ticks || ticks == -1) {
        // simulate first tic
        simulateTick();

        // print current status
        std::cout << toString() << "\n";
    }
}