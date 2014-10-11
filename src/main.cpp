#include "demo.hpp"
#include "runner.hpp"

#include <memory>

int main()
{
    WindowSetting window_setting({
        "Demo",
        800, 600,
        // resize
        true,
        // fullscreen
        false,
        // exit on close
        true,
        // exit on esc
        true
    });
    auto window = std::make_shared<Window>(window_setting);

    if (window->is_open()) {
        Demo demo(window, window_setting);

        Runner runner;
        return runner.control(window, demo);
    }

    return 1;
}
