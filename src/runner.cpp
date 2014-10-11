#include "runner.hpp"

#include "clock.hpp"
#include "input.hpp"

#include <iostream>

int Runner::control(std::shared_ptr<Window> window, World & world)
{
    if (!window->is_open()) {
        std::cerr << "Runner::control: window has not been opened." << std::endl;
        return 1;
    }

    if (!world.create()) {
        world.destroy();
        std::cerr << "Runner::control: could not created world without errors." << std::endl;
        return 1;
    }

    Clock clock;
    Input input;

    seconds dt;
    seconds elapsed;

    while (!window->should_close()) {
        window->poll(input);

        dt = clock.delta();
        elapsed = clock.elapsed();

        world.update(dt, elapsed, input);
        world.render(dt, elapsed);

        window->swap();
    }

    return 0;
}
