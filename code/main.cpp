#include <core/window_manager.hpp>

#include <opengl/functions_loader.hpp>
#include <opengl/vertex_array.hpp>
#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>

#include <math/vec3.hpp>

int32_t main()
{
    using namespace engine;

    core::Context::init_extensions();

    core::WindowManager::instance().create({ });
    core::WindowManager::instance().display();

    gl::FunctionsLoader::init_core();
    gl::FunctionsLoader::init_extensions();

    gl::Pipeline::enable(gl::framebuffer_srgb);

    const std::vector<math::vec3> vertices
    {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f }
    };

    gl::Buffer vertex_buffer;
    vertex_buffer.create();
    vertex_buffer.data(core::BufferData::create(vertices));

    gl::VertexArray vertex_array;
    vertex_array.create();
    vertex_array.attach_vertices(vertex_buffer, sizeof(math::vec3));

    while (core::WindowManager::instance().is_active())
    {
        gl::Commands::clear(1.0f, 0.5f, 0.0f);
        gl::Commands::clear(gl::color_buffer_bit);

        vertex_array.bind();

        gl::Commands::draw_arrays(gl::triangles, vertices.size());

        core::WindowManager::instance().update();
    }

    vertex_buffer.destroy();
    vertex_array.destroy();

    core::WindowManager::instance().destroy();

    return 0;
}