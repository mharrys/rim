#ifndef DEMO_HPP_INCLUDED
#define DEMO_HPP_INCLUDED

#include "framebuffer.hpp"
#include "fragmentshader.hpp"
#include "freelookcontrol.hpp"
#include "mesh.hpp"
#include "perspectivecamera.hpp"
#include "orthocamera.hpp"
#include "program.hpp"
#include "renderbuffer.hpp"
#include "texture2d.hpp"
#include "vertexshader.hpp"
#include "world.hpp"
#include "worldobject.hpp"

#include <memory>
#include <vector>

class Demo : public World {
public:
    Demo(std::shared_ptr<Window> window, WindowSetting window_setting);
    bool create() override;
    void update(seconds delta, seconds elapsed, Input & input) override;
    void render(seconds delta, seconds elapsed) override;
    void destroy() override;
private:
    void update_dimension();

    void create_quad();
    bool create_shaders();
    void render_pass1();
    void render_pass2();
    void render_pass3();

    std::shared_ptr<Window> window;
    int width;
    int height;

    VertexShader blinn_phong_vs, rim_light_vs;
    FragmentShader blinn_phong_fs, rim_light_fs;
    Program blinn_phong_program, rim_light_program, texture_program;

    Texture2D tex_normal, tex_rim;
    Renderbuffer rbo_depth;
    Framebuffer fbo_render;

    PerspectiveCamera camera;
    OrthoCamera ortho_camera;

    FreeLookControl camera_control;

    bool controls;
    bool spooky;

    float rim_width;
    int rim_color_choice;
    std::vector<glm::vec3> rim_colors;

    WorldObject model;
    glm::vec4 light_position;

    Mesh quad;
};

#endif
