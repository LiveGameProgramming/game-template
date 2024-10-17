#include <core/window_manager.hpp>

#include <opengl/functions_loader.hpp>
#include <opengl/commands.hpp>
#include <opengl/macros.hpp>

int32_t main()
{
    using namespace engine;

    core::Context::init_extensions();

    core::WindowManager::instance().create({ });
    core::WindowManager::instance().display();

    gl::FunctionsLoader::init_core();
    gl::FunctionsLoader::init_extensions();

    while (core::WindowManager::instance().is_active())
    {
        gl::Commands::clear(1.0f, 0.5f, 0.0f);
        gl::Commands::clear(gl::color_buffer_bit);

        core::WindowManager::instance().update();
    }

    core::WindowManager::instance().destroy();

    return 0;
}