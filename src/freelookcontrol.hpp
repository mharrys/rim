#ifndef FREELOOKCONTROL_HPP_INCLUDED
#define FREELOOKCONTROL_HPP_INCLUDED

#include "clock.hpp"

class Input;
class WorldObject;

class FreeLookControl {
public:
    FreeLookControl();
    void update(seconds dt, const Input & input, WorldObject & object);

    float window_width;
    float window_height;

    float rotation_speed;
    float movement_speed;
private:
    void rotate(seconds dt, const Input & input, WorldObject & object);
    void move(seconds dt, const Input & input, WorldObject & object);

    float yaw_angle;
    float pitch_angle;
};

#endif
