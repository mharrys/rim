#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include "button.hpp"
#include "key.hpp"

#include "lib/glm.hpp"

#include <array>

class Window;

enum class State {
    UP = 0,
    DOWN,
    PRESSED = 2,
    CLICKED = 2,
    DBLCLICKED,
    RELEASED
};

typedef std::array<State, 6> Buttons;
typedef std::array<State, 139> Keys;

struct Mouse {
    Buttons buttons;
    glm::ivec2 position;
    glm::ivec2 position_rel;
    int scroll_delta;
};

struct Keyboard {
    Keys keys;
};

class Input {
    friend Window;
public:
    Input();

    bool pressed(Key key) const;
    bool down(Key key) const;
    bool released(Key key) const;
    bool up(Key key) const;

    bool clicked(Button button) const;
    bool dblclicked(Button button) const;
    bool down(Button button) const;
    bool released(Button button) const;
    bool up(Button button) const;

    glm::ivec2 position() const;
    glm::ivec2 position_rel() const;
    int scroll_delta() const;

    State state(Button button) const;
    State state(Key key) const;
private:
    void update();

    void set_state(Button button, State state);
    void set_state(Key key, State state);

    Mouse mouse;
    Keyboard keyboard;
};

#endif
