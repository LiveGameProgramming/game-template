#include <core/window_factory.hpp>

int32_t main()
{
    using namespace engine::core;

    const auto factory = WindowFactory::create();
    const auto window  = factory->create_window();
    const auto events  = factory->create_events();
    const auto context = factory->create_context();

    window->title("Game Template").create();
    context->create(window->handle());

    window->display();

    while (true)
    {
        context->update();
        events->update();
    }

    context->destroy();
    window->destroy();

    return 0;
}