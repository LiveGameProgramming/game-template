#include <core/window_factory.hpp>

int32_t main()
{
    using namespace engine::core;

    const auto factory = WindowFactory::create();
    const auto window  = factory->create_window();
    const auto events  = factory->create_events();

    window->title("Game Template")->create();
    window->display();

    while (true)
    {
        events->update();
    }

    window->destroy();

    return 0;
}