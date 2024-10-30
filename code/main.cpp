#include <base/window_manager.hpp>
#include <base/file.hpp>
#include <base/mat4.hpp>
#include <base/time.hpp>

#include <opengl/functions_loader.hpp>
#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>

#include <opengl/vertex_array.hpp>
#include <opengl/shader.hpp>

#include <math/functions.hpp>

int32_t main()
{
    using namespace engine;

    base::Context::init_extensions();

    base::WindowManager::instance().create({ .size = { 1280, 1024 } }, { });
    base::WindowManager::instance().display();

    opengl::FunctionsLoader::init_core();
    opengl::FunctionsLoader::init_extensions();

    opengl::Pipeline::enable(opengl::framebuffer_srgb);
    opengl::Pipeline::enable(opengl::multisample);
    opengl::Pipeline::enable(opengl::cull_face);

    #pragma region Shaders

    opengl::ShaderStage vertex_stage { opengl::vertex_stage };
    vertex_stage.create();
    vertex_stage.source(base::File::read("default_shader.vert", std::ios::binary));

    opengl::ShaderStage fragment_stage { opengl::fragment_stage };
    fragment_stage.create();
    fragment_stage.source(base::File::read("default_shader.frag", std::ios::binary));

    opengl::Shader default_shader;
    default_shader.create();
    default_shader.attach(vertex_stage);
    default_shader.attach(fragment_stage);
    default_shader.link();

    vertex_stage.destroy();
    fragment_stage.destroy();

    #pragma endregion

    const std::vector<base::vec3> vertices
    {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    const std::vector<uint32_t> indices
    {
        2, 1, 0,
        0, 3, 2
    };

    opengl::Buffer vertex_buffer;
    vertex_buffer.create();
    vertex_buffer.data(base::buffer::data::create(vertices));

    opengl::Buffer index_buffer;
    index_buffer.create();
    index_buffer.data(base::buffer::data::create(indices));

    opengl::VertexArray vertex_array;
    vertex_array.create();
    vertex_array.attach_vertices(vertex_buffer, sizeof(base::vec3));
    vertex_array.attach_indices(index_buffer);
    vertex_array.attribute({ 0, 3, opengl::type_float });

    const float aspect_ratio = static_cast<float>(base::WindowManager::instance().width()) /
                               static_cast<float>(base::WindowManager::instance().height());
    base::mat4 proj;
    proj.perspective(60.0f, aspect_ratio, 0.1f, 100.0f);

    base::mat4 view;
    view.look_at({ 0.0f, 0.0f, 5.0f }, { }, { 0.0f, 1.0f, 0.0f });

    #pragma region Uniform Buffers

    const std::vector camera_data { view, proj };

    opengl::Buffer camera_ubo;
    camera_ubo.create();
    camera_ubo.bind();
    camera_ubo.data(base::buffer::data::create(camera_data));

    #pragma endregion

    base::Time time;
    time.init();

    while (base::WindowManager::instance().is_active())
    {
        time.update();

        const float r = 5.0f;
        const float x = math::sin(base::Time::total_time()) * -r;
        const float z = math::cos(base::Time::total_time()) *  r;

        view.look_at({ x, 0.0f, z }, { }, { 0.0f, 1.0f, 0.0f });

        camera_ubo.sub_data(base::buffer::data::create(&view));

        opengl::Commands::clear(1.0f, 0.5f, 0.0f);
        opengl::Commands::clear(opengl::color_buffer_bit);

        default_shader.bind();

        vertex_array.bind();

        base::mat4 model;
        model.identity();

        default_shader.push_mat4(0, model);

        opengl::Commands::draw_indexed(opengl::triangles, static_cast<int32_t>(indices.size()));

        base::WindowManager::instance().update();
    }

    index_buffer.destroy();
    vertex_buffer.destroy();
    vertex_array.destroy();

    default_shader.destroy();

    base::WindowManager::instance().destroy();

    return 0;
}