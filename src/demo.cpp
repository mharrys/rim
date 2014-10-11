#include "demo.hpp"

#include "meshloader.hpp"

#include "lib/gl.hpp"

#include <iostream>

Demo::Demo(std::shared_ptr<Window> window, WindowSetting window_setting)
    : window(window),
      width(window_setting.width),
      height(window_setting.height),
      controls(true),
      spooky(false),
      rim_width(0.8f),
      rim_color_choice(0),
      light_position(0.0f, 0.5f, 4.0f, 1.0f)
{
}

bool Demo::create()
{
    window->request_pointer_lock(true);

    std::string path = "assets/models/suzanne.obj";
    if (!MeshLoader::load(path, model)) {
        std::cerr << "Demo::on_create: unable to load model \"" << path << "\"." << std::endl;
        return false;
    }
    model.update_world_transform();

    if (!create_shaders()) {
        return false;
    }

    tex_normal.bind();
    tex_normal.make(width, height);

    tex_rim.bind();
    tex_rim.make(width, height);

    rbo_depth.width = width;
    rbo_depth.height = height;
    rbo_depth.make();

    GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };

    fbo_render.bind();
    fbo_render.attach_depth(BindTarget::FRAMEBUFFER, rbo_depth);
    glDrawBuffers(1, draw_buffers);
    Framebuffer::status();

    camera.aspect_ratio = width / static_cast<float>(height);
    camera.translate_z(3.5f);
    camera.update_world_transform();

    camera_control.movement_speed = 3.5f;
    camera_control.window_width = width;
    camera_control.window_height = height;

    create_quad();

    rim_colors = {
        glm::vec3(0.6f, 0.6f, 0.4f), // yellow tint
        glm::vec3(0.0f, 1.0f, 0.0f), // green
        glm::vec3(0.0f, 0.8f, 0.9f), // teal
        glm::vec3(0.8f, 0.0f, 0.8f), // pink
    };

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return true;
}

void Demo::update(seconds dt, seconds, Input & input)
{
    update_dimension();

    if (blinn_phong_vs.is_modified() || blinn_phong_fs.is_modified()) {
        blinn_phong_vs.recompile();
        blinn_phong_fs.recompile();
        blinn_phong_program.attach(blinn_phong_vs, blinn_phong_fs);
    }

    if (rim_light_vs.is_modified() || rim_light_fs.is_modified()) {
        rim_light_vs.recompile();
        rim_light_fs.recompile();
        rim_light_program.attach(rim_light_vs, rim_light_fs);
    }

    if (input.pressed(Key::F1)) {
        controls = !controls;
        window->request_pointer_lock(controls);
    }

    if (input.pressed(Key::F2)) {
        spooky = !spooky;
    }

    if (input.pressed(Key::PLUS)) {
        rim_width += 0.05f;
    }

    if (input.pressed(Key::MINUS)) {
        rim_width -= 0.05f;
    }

    if (input.pressed(Key::D1)) {
        rim_color_choice = 0;
    }

    if (input.pressed(Key::D2)) {
        rim_color_choice = 1;
    }

    if (input.pressed(Key::D3)) {
        rim_color_choice = 2;
    }

    if (input.pressed(Key::D4)) {
        rim_color_choice = 3;
    }

    rim_width = glm::clamp(rim_width, 0.0f, 1.0f);

    if (controls) {
        camera_control.update(dt, input, camera);
        camera.update_world_transform();
    }
}

void Demo::render(seconds, seconds)
{
    glEnable(GL_DEPTH_TEST);
    render_pass1();
    render_pass2();

    glDisable(GL_DEPTH_TEST);
    render_pass3();

    gl_print_error();
}

void Demo::destroy()
{
    window->request_pointer_lock(false);
}

void Demo::update_dimension()
{
    auto dimension = window->dimension();
    if (width != dimension.first || height != dimension.second) {
        width = dimension.first;
        height = dimension.second;

        glViewport(0, 0, width, height);

        camera.aspect_ratio = width / static_cast<float>(height);

        camera_control.window_width = width;
        camera_control.window_height = height;

        tex_normal.bind();
        tex_normal.make(width, height);

        tex_rim.bind();
        tex_rim.make(width, height);

        rbo_depth.width = width;
        rbo_depth.height = height;
        rbo_depth.make();
    }
}

void Demo::create_quad()
{
    const float w = 1.0f;
    const float h = 1.0f;
    quad.positions = {
        glm::vec3(-w, -h, 0.0f),
        glm::vec3( w, -h, 0.0f),
        glm::vec3(-w,  h, 0.0f),
        glm::vec3( w,  h, 0.0f),
    };
    quad.indices = {
        0, 1, 2,
        2, 1, 3,
    };
    quad.update_positions = true;
    quad.update_indices = true;
}

bool Demo::create_shaders()
{
    blinn_phong_vs.compile_from_file("assets/shaders/blinnphong.vs");
    blinn_phong_fs.compile_from_file("assets/shaders/blinnphong.fs");
    if (!blinn_phong_program.attach(blinn_phong_vs, blinn_phong_fs)) {
        return false;
    }

    rim_light_vs.compile_from_file("assets/shaders/rimlight.vs");
    rim_light_fs.compile_from_file("assets/shaders/rimlight.fs");
    if (!rim_light_program.attach(rim_light_vs, rim_light_fs)) {
        return false;
    }

    VertexShader texture_vs;
    texture_vs.compile_from_file("assets/shaders/texture.vs");
    FragmentShader texture_fs;
    texture_fs.compile_from_file("assets/shaders/texture.fs");
    if (!texture_program.attach(texture_vs, texture_fs)) {
        return false;
    }

    return true;
}

void Demo::render_pass1()
{
    blinn_phong_program.use();

    glm::mat4 m = model.world_transform();
    glm::mat4 v = camera.view();
    glm::mat4 p = camera.projection();

    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv));

    glm::vec4 light_position_es = v * light_position;

    blinn_phong_program.set_uniform("mv", mv);
    blinn_phong_program.set_uniform("mvp", mvp);
    blinn_phong_program.set_uniform("nm", nm);
    blinn_phong_program.set_uniform("light_position", light_position_es);

    fbo_render.bind();
    fbo_render.attach_color(BindTarget::FRAMEBUFFER, tex_normal);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model.traverse([&](WorldObject & object) {
        object.draw();
    });

    Framebuffer::status();
    Framebuffer::unbind();
}

void Demo::render_pass2()
{
    rim_light_program.use();

    glm::mat4 m = model.world_transform();
    glm::mat4 v = camera.view();
    glm::mat4 p = camera.projection();

    glm::mat4 mv = v * m;
    glm::mat4 mvp = p * mv;
    glm::mat3 nm = glm::inverseTranspose(glm::mat3(mv));

    rim_light_program.set_uniform("mv", mv);
    rim_light_program.set_uniform("mvp", mvp);
    rim_light_program.set_uniform("nm", nm);
    rim_light_program.set_uniform("rim_color", rim_colors[rim_color_choice]);
    rim_light_program.set_uniform("rim_width", rim_width);
    rim_light_program.set_uniform("alpha", spooky ? 0.5f : 1.0f);

    fbo_render.bind();
    fbo_render.attach_color(BindTarget::FRAMEBUFFER, tex_rim);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    model.traverse([&](WorldObject & object) {
        object.draw();
    });

    glDisable(GL_BLEND);

    Framebuffer::status();
    Framebuffer::unbind();
}

void Demo::render_pass3()
{
    texture_program.use();

    glm::mat4 v = ortho_camera.view();
    glm::mat4 p = ortho_camera.projection();
    glm::mat4 mvp = p * v;

    texture_program.set_uniform("mvp", mvp);

    glClear(GL_COLOR_BUFFER_BIT);

    if (!spooky) {
        tex_normal.bind();
        quad.draw();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }

    tex_rim.bind();
    quad.draw();

    if (!spooky) {
        glDisable(GL_BLEND);
    }
}
