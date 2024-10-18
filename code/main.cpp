#include <core/window_manager.hpp>

#include <opengl/functions_loader.hpp>
#include <opengl/commands.hpp>
#include <opengl/pipeline.hpp>
#include <opengl/buffer.hpp>

struct vec3D
{
    float x;
    float y;
    float z;
};

int32_t main()
{
    using namespace engine;

    core::Context::init_extensions();

    core::WindowManager::instance().create({ });
    core::WindowManager::instance().display();

    gl::FunctionsLoader::init_core();
    gl::FunctionsLoader::init_extensions();

    gl::Pipeline::enable(gl::framebuffer_srgb);

    const std::vector<vec3D> vertices
    {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f }
    };

    gl::Buffer vertex_buffer;
    vertex_buffer.create();
    vertex_buffer.data(core::BufferData::create(vertices));

    while (core::WindowManager::instance().is_active())
    {
        gl::Commands::clear(1.0f, 0.5f, 0.0f);
        gl::Commands::clear(gl::color_buffer_bit);

        gl::Commands::draw_arrays(gl::triangles, vertices.size());

        core::WindowManager::instance().update();
    }

    vertex_buffer.destroy();

    core::WindowManager::instance().destroy();

    return 0;
}