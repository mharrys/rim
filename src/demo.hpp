#ifndef DEMO_HPP_INCLUDED
#define DEMO_HPP_INCLUDED

#include "assets.hpp"

#include "gust.hpp"

class Demo : public gst::World {
public:
    Demo(std::shared_ptr<gst::Logger> logger, std::shared_ptr<gst::Window> window);
    bool create() final;
    void update(float delta, float elapsed) final;
    void destroy() final;
private:
    void create_scene();
    void create_model();
    void update_input(float delta);

    std::shared_ptr<gst::Logger> logger;
    std::shared_ptr<gst::Window> window;

    gst::Renderer renderer;
    gst::Scene scene;

    gst::Resolution render_size;
    gst::ProgramPool programs;

    gst::FirstPersonControl controls;
};

#endif
