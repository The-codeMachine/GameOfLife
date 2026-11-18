#include <SDL3/SDL_main.h>

#include <World.hpp>
#include <rendering/Engine.hpp>

#include <iostream>
#include <thread>
#include <atomic>

int main(int argc, char* argv[]) {

    World w(20, 20, 0.5);
    Engine e(w);

    std::atomic<bool> running = true;

    std::thread simulationThread([&]() {
        w.run();
    });

    std::thread renderingThread([&]() {
        e.render();
    });

    std::string in;

    while (running && std::getline(std::cin, in)) {
        if (in == "stop") {
            running = false;
        }
    }

    // Signal both threads to terminate.
    e.stop();
    w.stop();

    // Wait for both threads to finish.
    if (renderingThread.joinable()) {
        renderingThread.join();
    }

    if (simulationThread.joinable()) {
        simulationThread.join();
    }

    return 0;
}