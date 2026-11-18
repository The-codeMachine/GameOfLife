#include <World.hpp>
#include <utils/positionConversions.hpp>

#include <random>
#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <cstdint>
#include <thread>

void World::fillWorld(float temperature) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Clamp temperature to the valid probability range.
    temperature = std::clamp(temperature, 0.0f, 1.0f);

    std::bernoulli_distribution distrib(temperature);

    const uint64_t width = m_xBounds;
    const uint64_t height = m_yBounds;

    if (width == 0 || height == 0) {
        m_cells.clear();
        return;
    }

    if (width * height > std::numeric_limits<size_t>::max()) {
        throw std::length_error("World dimensions are too large.");
    }

    const uint64_t indexMax = width * height;
    m_cells.resize(static_cast<size_t>(indexMax));

    for (uint64_t i = 0; i < indexMax; ++i) {
        const auto [x, y] = convert1Dto2D(i, m_xBounds);

        m_cells[i] = {
            distrib(gen),
            static_cast<uint32_t>(x),
            static_cast<uint32_t>(y)
        };
    }
}

uint16_t World::getNeighbours(Cell cell) const noexcept {
    // Exclude the cell itself.
    const uint32_t xStart = (cell.x > 0) ? cell.x - 1 : 0;
    const uint32_t yStart = (cell.y > 0) ? cell.y - 1 : 0;

    const uint32_t xEnd = std::min(cell.x + 1, m_xBounds - 1);
    const uint32_t yEnd = std::min(cell.y + 1, m_yBounds - 1);

    uint16_t count = 0;
    for (uint32_t y = yStart; y <= yEnd; ++y) {
        for (uint32_t x = xStart; x <= xEnd; ++x) {
            if (x == cell.x && y == cell.y) {
                continue;
            }

            count += m_cells[convert2Dto1D(x, y, m_xBounds)].alive;
        }
    }

    return count;
}

bool World::isCellAlive(Cell cell) const noexcept {
    const uint16_t aliveCounter = getNeighbours(cell);

    // Conway's Game of Life:
    // A live cell survives with 2 or 3 neighbours.
    // A dead cell becomes alive with exactly 3 neighbours.

    if (cell.alive) {
        return aliveCounter == 2 || aliveCounter == 3;
    }

    return aliveCounter == 3;
}

void World::simulateTick() {
    std::vector<Cell> nextGeneration = m_cells;

    for (size_t i = 0; i < m_cells.size(); ++i) {
        nextGeneration[i].alive = isCellAlive(m_cells[i]);
    }

    m_cells.swap(nextGeneration);
}

World::World(uint32_t xBounds, uint32_t yBounds, float temperature)
    : m_xBounds(xBounds),
      m_yBounds(yBounds)
{
    fillWorld(temperature);
}

std::vector<Cell> World::cells() {
    return m_cells;
}

const std::vector<Cell>& World::cells() const noexcept {
    return m_cells;
}

void World::run(int ticks, std::optional<std::chrono::milliseconds> pauseTime) {
    // Enter alternate screen buffer and hide cursor
    std::cout << "\033[?1049h\033[?25l";

    uint64_t i = 0;

    while (ticks == -1 || i < static_cast<uint64_t>(ticks)) {
        // Jump directly to top-left corner (row 1, col 1)
        std::cout << "\033[HCurrent State (Tick " << i << "):\n" << toString() << std::flush;

        simulateTick();

        // pauses the simulation for the specified time 
        if (pauseTime.has_value()) {
            std::this_thread::sleep_for(*pauseTime);
        }

        ++i;
    }

    // Show cursor and exit alternate screen buffer
    std::cout << "\033[?25h\033[?1049l" << std::flush;

    // print final result
    std::cout << "Final State (Tick " << i << "):\n" << toString() << std::flush;
}

std::string World::toString() const noexcept {
    std::string result;

    for (uint32_t y = 0; y < m_yBounds; ++y) {
        for (uint32_t x = 0; x < m_xBounds; ++x) {
            const Cell& cell = m_cells[convert2Dto1D(x, y, m_xBounds)];

            result += cell.alive ? "." : " ";
        }

        result += '\n';
    }

    return result;
}
