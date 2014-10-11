#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "clock.hpp"
#include "input.hpp"
#include "window.hpp"

class World {
public:
    virtual bool create();
    virtual void update(seconds delta, seconds elapsed, Input & input);
    virtual void render(seconds delta, seconds elapsed);
    virtual void destroy();
};

#endif
