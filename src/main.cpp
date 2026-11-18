#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <World.hpp>

#include <rendering/Engine.hpp>

int main(int argc, char* argv[]) {

    // Test engine
    World w(100, 100, 1);
    Engine e(w);

    e.render();

    /*

    World world(100, 60, 0.6);

    // infinite simulation with 1 second pause
    world.run(-1, std::chrono::milliseconds(500));

    */

    return 0;
};