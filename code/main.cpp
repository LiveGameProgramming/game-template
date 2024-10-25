#include <core/window_manager.hpp>
#include <core/file.hpp>

#include <opengl/functions_loader.hpp>
#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>

#include <opengl/vertex_array.hpp>
#include <opengl/shader.hpp>

#include <math/mat4.hpp>

int32_t main()
{
    using namespace engine;

    core::Context::init_extensions();

    core::WindowManager::instance().create({ .size = { 1280, 1024 } }, { });
    core::WindowManager::instance().display();

    gl::FunctionsLoader::init_core();
    gl::FunctionsLoader::init_extensions();

    gl::Pipeline::enable(gl::framebuffer_srgb);
    gl::Pipeline::enable(gl::multisample);

    #pragma region Shaders

    gl::ShaderStage vertex_stage { gl::vertex_stage };
    vertex_stage.create();
    vertex_stage.source(core::File::read("default_shader.vert", std::ios::binary));

    gl::ShaderStage fragment_stage { gl::fragment_stage };
    fragment_stage.create();
    fragment_stage.source(core::File::read("default_shader.frag", std::ios::binary));

    gl::Shader default_shader;
    default_shader.create();
    default_shader.attach(vertex_stage);
    default_shader.attach(fragment_stage);
    default_shader.link();

    vertex_stage.destroy();
    fragment_stage.destroy();

    #pragma endregion

    const std::vector<math::vec3> vertices
    {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    const std::vector<uint32_t> indices
    {
        0, 1, 2,
        2, 3, 0
    };

    gl::Buffer vertex_buffer;
    vertex_buffer.create();
    vertex_buffer.data(core::BufferData::create(vertices));

    gl::Buffer index_buffer;
    index_buffer.create();
    index_buffer.data(core::BufferData::create(indices));

    gl::VertexArray vertex_array;
    vertex_array.create();
    vertex_array.attach_vertices(vertex_buffer, sizeof(math::vec3));
    vertex_array.attach_indices(index_buffer);
    vertex_array.attribute({ 0, 3, gl::type_float });

    const float aspect_ratio = static_cast<float>(core::WindowManager::instance().width()) /
                               static_cast<float>(core::WindowManager::instance().height());
    math::mat4 proj;
    proj.perspective(45.0f, aspect_ratio, 0.1f, 100.0f);

    math::mat4 view;
    view.identity();
    view.translate({ 0.0f, 0.0f, -5.0f });

    while (core::WindowManager::instance().is_active())
    {
        gl::Commands::clear(1.0f, 0.5f, 0.0f);
        gl::Commands::clear(gl::color_buffer_bit);

        default_shader.bind();

        vertex_array.bind();

        math::mat4 model;
        model.identity();
        model.translate({ 1.0f, 0.0f, 0.0f });
        model.scale({ 0.5f });

        default_shader.push_mat4(0, proj * view * model);

        gl::Commands::draw_indexed(gl::triangles, static_cast<int32_t>(indices.size()));

        core::WindowManager::instance().update();
    }

    index_buffer.destroy();
    vertex_buffer.destroy();
    vertex_array.destroy();

    default_shader.destroy();

    core::WindowManager::instance().destroy();

    return 0;
}