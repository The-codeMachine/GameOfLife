#include <SDL3/SDL_main.h>

#include <World.hpp>
#include <rendering/Engine.hpp>

#include <cstdint>
#include <cstring>
#include <charconv>

#include <thread>

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        SDL_Log(
            "Need at least 4 arguments; World xBounds, World yBounds, probability, and simulation speed"
        );
        return 1;
    }
    
    uint32_t xBounds = 100;
    uint32_t yBounds = 100;
    float prob = 0.5;
    size_t speed = 500; // in seconds

    std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), xBounds);
    std::from_chars(argv[2], argv[2] + std::strlen(argv[2]), yBounds);
    std::from_chars(argv[3], argv[3] + std::strlen(argv[3]), prob);
    std::from_chars(argv[4], argv[4] + std::strlen(argv[4]), speed);

    World w(xBounds, yBounds, prob);
    Engine e(w);

    std::thread simulationThread([&w, &speed]() {
        w.run(speed);
    });

    // SDL rendering and event processing on the main thread.
    e.render();

    // Stop the simulation when the window closes.
    w.stop();

    if (simulationThread.joinable()) {
        simulationThread.join();
    }

    return 0;
}