#include <World.hpp>
#include <utils/positionConversions.hpp>

#include <random>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cstdint>

#include <mutex>
#include <thread>

void World::fillWorld(float temperature) {
    // Clamp temperature to the valid probability range.
    temperature = std::clamp(temperature, 0.0f, 1.0f);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::bernoulli_distribution distrib(temperature);

    const uint64_t width = m_xBounds;
    const uint64_t height = m_yBounds;

    if (height != 0 && width > std::numeric_limits<size_t>::max() / height) {
        throw std::length_error("World dimensions are too large.");
    }

    const size_t cellCount = width * height;
    std::vector<Cell> newCells(cellCount);

    for (size_t i = 0; i < cellCount; ++i) {
        const auto [x, y] = convert1Dto2D(i, m_xBounds);

        newCells[i] = {
            distrib(gen),
            static_cast<uint32_t>(x),
            static_cast<uint32_t>(y)
        };
    }

    {
        std::unique_lock loc(m_cellsMutex);
        m_cells.swap(newCells);
    }
}

uint16_t World::getNeighboursUnlocked(Cell cell) const {
    if (m_xBounds == 0 || m_yBounds == 0 ||
        cell.x >= m_xBounds || cell.y >= m_yBounds) {
        return 0;
    }

    const uint32_t xStart = cell.x > 0 ? cell.x - 1 : 0;
    const uint32_t yStart = cell.y > 0 ? cell.y - 1 : 0;

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

bool World::isCellAliveUnlocked(Cell cell) const {
    const uint16_t aliveCounter = getNeighboursUnlocked(cell);

    // Conway's Game of Life:
    // A live cell survives with 2 or 3 neighbours.
    // A dead cell becomes alive with exactly 3 neighbours.

    if (cell.alive) {
        return aliveCounter == 2 || aliveCounter == 3;
    }

    return aliveCounter == 3;
}
 
uint16_t World::getNeighbours(Cell cell) const {
    std::shared_lock lock(m_cellsMutex);
    return getNeighboursUnlocked(cell);
}

bool World::isCellAlive(Cell cell) const {
    std::shared_lock lock(m_cellsMutex);
    return isCellAliveUnlocked(cell);
}

void World::simulateTick() {
    std::unique_lock lock(m_cellsMutex);

    std::vector<Cell> nextGeneration = m_cells;
    size_t cellsSize = m_cells.size();

    for (size_t i = 0; i < cellsSize; ++i) {
        nextGeneration[i].alive = isCellAliveUnlocked(m_cells[i]);
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
    std::shared_lock lock(m_cellsMutex);
    return m_cells;
}

const std::vector<Cell>& World::cells() const noexcept {
    std::shared_lock lock(m_cellsMutex);
    return m_cells;
}

void World::run(int ticks, std::optional<std::chrono::milliseconds> pauseTime) {
    if (ticks < -1) {
        throw std::invalid_argument("ticks must be -1 or non-negative.");
    }

    m_running.store(true, std::memory_order_relaxed);

    for (int i = 0; ticks == -1 || i < ticks; ++i) {
        if (!m_running.load(std::memory_order_relaxed)) {
            break;
        }

        simulateTick();

        if (pauseTime.has_value() && *pauseTime > std::chrono::milliseconds::zero()) {
            std::this_thread::sleep_for(*pauseTime);
        }
    }

    m_running.store(false, std::memory_order_relaxed);
}

void World::stop() {
    m_running.store(false, std::memory_order_relaxed);
}

std::string World::toString() const noexcept {
    std::shared_lock lock(m_cellsMutex);

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
