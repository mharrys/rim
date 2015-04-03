#include "demo.hpp"

Demo::Demo(std::shared_ptr<gst::Logger> logger, std::shared_ptr<gst::Window> window)
    : logger(logger),
      window(window),
      renderer(gst::Renderer::create(logger)),
      controls(true, 2.5f, 2.5f)
{
}

bool Demo::create()
{
    window->set_pointer_lock(true);

    create_scene();
    create_model();

    return true;
}

void Demo::update(float delta, float)
{
    update_input(delta);
    scene.update();

    renderer.render(scene);
}

void Demo::destroy()
{
    window->set_pointer_lock(false);
}

void Demo::create_scene()
{
    scene = gst::Scene::create_perspective({ 45.0f, window->get_size(), 0.1f, 1000.0f });
    scene.get_eye().position = glm::vec3(0.0f, 0.0f, 4.5f);
}

void Demo::create_model()
{
    gst::ProgramPool programs(logger);
    auto rim_light_program = programs.create(RIMLIGHT_VS, RIMLIGHT_FS);

    auto pass = std::make_shared<gst::ShadedPass>(rim_light_program);
    pass->set_cull_face(gst::CullFace::BACK);
    pass->set_depth_test(true);
    pass->set_skip_light(true);

    auto material = gst::Material::create_struct("material");
    material.get_uniform("diffuse") = glm::vec3(0.0f, 0.1f, 0.0f);
    material.get_uniform("rim.color") = glm::vec3(0.6f, 0.6f, 0.4f);
    material.get_uniform("rim.width") = 0.7f;
    material.get_uniform("rim.alpha") = 1.0f;

    gst::MeshFactory mesh_factory(logger);
    for (auto mesh : mesh_factory.create_from_file(SUZANNE_OBJ)) {
        auto model = gst::Model(mesh, material, pass);
        auto model_node = std::make_shared<gst::ModelNode>(model);
        scene.add(model_node);
    }
}

void Demo::update_input(float delta)
{
    const auto input = window->get_input();

    controls.update(delta, input, scene.get_eye());
}
