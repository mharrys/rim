#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include "input.hpp"

#include <memory>
#include <string>
#include <utility>

class Runner;

struct NativeWindow;

struct WindowSetting {
    std::string title;
    int width;
    int height;
    bool resize;
    bool fullscreen;
    bool exit_on_close;
    bool exit_on_esc;
};

class Window {
    friend Runner;
public:
    Window(WindowSetting setting);
    ~Window();

    void request_pointer_lock(bool pointer_lock);
    bool has_pointer_lock() const;

    bool is_open() const;
    bool should_close() const;

    std::pair<int, int> dimension() const;
private:
    bool open();
    void close();

    void poll(Input & input);
    void swap();

    void reset();

    std::unique_ptr<NativeWindow> native;

    std::string title;

    int width;
    int height;

    bool resize;
    bool fullscreen;
    bool exit_on_close;
    bool exit_on_esc;
    bool exit_flag;
    bool pointer_lock;
};

#endif
