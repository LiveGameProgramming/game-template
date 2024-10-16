#include <core/window_manager.hpp>

int32_t main()
{
    using namespace engine::core;

    WindowManager::instance().create({ });
    WindowManager::instance().display();

    while (WindowManager::instance().is_active())
    {
        //glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        WindowManager::instance().update();
    }

    WindowManager::instance().destroy();

    return 0;
}