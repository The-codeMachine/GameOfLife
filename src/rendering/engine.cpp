#include <rendering/Engine.hpp>

#include <SDL3/SDL_log.h>

Engine::Engine(World& world, const uint32_t width, const uint32_t height) : world(world) 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("SDL_INIT failed");
    }

    if (!SDL_CreateWindowAndRenderer("Conway's Game Of Life", width, height, 0, &m_window, &m_renderer)) {
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

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        SDL_RenderPresent(m_renderer);
    }
}