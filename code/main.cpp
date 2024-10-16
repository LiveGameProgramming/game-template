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
        //glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        context->update();
        events->update();
    }

    context->destroy();
    window->destroy();

    return 0;
}