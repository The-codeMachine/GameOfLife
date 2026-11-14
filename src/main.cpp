#include <World.hpp>

int main() {

    World world(50, 10, 0.5);

    // infinite simulation with 1 second pause
    world.run(-1, std::chrono::milliseconds(500));

    return 0;
};