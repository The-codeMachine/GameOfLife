#include <rendering/Engine.hpp>

#include <SDL3/SDL_log.h>

CellSprite Engine::createCellRenderingObject(Cell cell) {
    // for the border (so each square is 20 pixels with its border)
    static constexpr uint32_t SIDE_LENGTH = 19; // pixels
    
    //                          x position                              y position                          width       height
    SDL_FRect frect = {static_cast<float>(cell.x * SIDE_LENGTH), static_cast<float>(cell.y * SIDE_LENGTH), SIDE_LENGTH, SIDE_LENGTH};

    return {frect, cell.alive};
}

std::vector<CellSprite> Engine::createCellsRenderingObject() {
    const std::vector<Cell>& cells = m_world.cells();
    size_t cellsSize = cells.size();
    
    std::vector<CellSprite> out(cellsSize); 

    for (uint64_t i = 0; i < cellsSize; ++i) {
        out[i] = createCellRenderingObject(cells[i]); 
    }

    return out;
}

void Engine::drawBorderedFRect(const SDL_FRect* rect, SDL_Color fillColor, SDL_Color borderColor) {
    // fill interior
    SDL_SetRenderDrawColor(m_renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(m_renderer, rect);

    // draw border
    SDL_SetRenderDrawColor(m_renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderRect(m_renderer, rect);
}

void Engine::drawCells() {
    const std::vector<CellSprite> cells = createCellsRenderingObject();
    const uint64_t cellsSize = cells.size();
    
    static constexpr SDL_Color fillColorAlive = {255, 255, 255, 1};
    static constexpr SDL_Color fillColorDead = {100, 100, 100, 1};
    static constexpr SDL_Color borderColor = {0, 0, 0, 1};

    for (uint64_t i = 0; i < cellsSize; ++i) {
        const CellSprite& sprite = cells[i];
        
        drawBorderedFRect(&sprite.frect, sprite.alive ? fillColorAlive : fillColorDead, borderColor);
    }
}


Engine::Engine(World& world, const uint32_t width, const uint32_t height) : m_world(world) 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("SDL_INIT failed");
    }

    if (!SDL_CreateWindowAndRenderer("Conway's Game Of Life", width, height, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        throw std::runtime_error("SDL_CreateWindowAndRenderer failed");
    }
}

Engine::~Engine() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void Engine::render() {
    bool running = true;
    SDL_Event event;

    while (running) {
        // Event handling (escape key, exit button)

        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
                break;

            default:
                break;
            }
        }


        SDL_RenderClear(m_renderer);

        drawCells();

        SDL_RenderPresent(m_renderer);
    }
}