#include <SDL3/SDL_main.h>

#include <World.hpp>
#include <rendering/Engine.hpp>

#include <thread>

int main(int argc, char* argv[]) {
    World w(1000, 1000, 0.5);
    Engine e(w);

    std::thread simulationThread([&w]() {
        w.run();
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