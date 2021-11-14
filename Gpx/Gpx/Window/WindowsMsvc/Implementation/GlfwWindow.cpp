
#include "../GlfwWindow.hpp"

namespace gpx
{

glfw_window::glfw_window(const window_desc& desc) :
    window(),
    _desc(desc),
    _ctx(nullptr)
{
    AUTO_TRACE;
}

glfw_window::~glfw_window()
{
    AUTO_TRACE;
}

boole glfw_window::start()
{
    AUTO_TRACE;

    if (glfwInit() == GLFW_FALSE)
    {
        return boole::False;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto* ctx = glfwCreateWindow(
        _desc.width, _desc.height, _desc.name.data(), nullptr, nullptr);

    if (ctx)
    {
        if (_ctx.compare_exchange(nullptr, ctx) == nullptr)
        {
            return boole::True;
        }
        else
        {
            glfwDestroyWindow(ctx);
            return boole::False;
        }
    }
    else
    {
        glfwTerminate();
        return boole::False;
    }
}

boole glfw_window::stop()
{
    AUTO_TRACE;

    auto* ctx = _ctx.exchange(nullptr);
    assert(ctx);

    if (ctx)
    {
        glfwDestroyWindow(ctx);
        glfwTerminate();
    }
    return boole::True;
}

boole glfw_window::present(s64 my_image)
{
    return boole::True;
}

boole glfw_window::poll_event()
{
    AUTO_TRACE;

    glfwPollEvents();
    return boole::True;
}

boole glfw_window::is_running()
{
    return _ctx.get() != nullptr;
}

}