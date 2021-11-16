
#include "../Interface.hpp"

namespace gpx
{

class runtime;

mutex glfw_window::_glfw_op_lock = mutex(boole::True);
s64 glfw_window::_window_number = 0;

glfw_window::glfw_window(const window_desc& desc, obs<runtime> rt) :
    window(),
    _desc(desc),
    _rt(rt),
    _ctx(nullptr)
{
    AUTO_TRACE;
}

glfw_window::~glfw_window()
{
    AUTO_TRACE;

    stop();
}

string glfw_window::name()
{
    return _desc.name;
}

boole glfw_window::start()
{
    AUTO_TRACE;

    _glfw_op_lock.wait_acquire();
    escape_function ef = [=]() mutable
    {
        _glfw_op_lock.release();
    };

    if (++_window_number == 1)
    {
        if (glfwInit() == GLFW_FALSE)
        {
            return boole::False;
        }
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto* ctx = glfwCreateWindow(
        _desc.width, _desc.height, _desc.name.data(), nullptr, nullptr);

    if (ctx && _ctx.compare_exchange(nullptr, ctx) == nullptr)
    {
        return boole::True;
    }
    if (ctx)
    {
        glfwDestroyWindow(ctx);
    }
    if (--_window_number == 0)
    {
        glfwTerminate();
    }
    return boole::False;
}

boole glfw_window::stop()
{
    AUTO_TRACE;

    _glfw_op_lock.wait_acquire();
    escape_function ef = [=]() mutable
    {
        _glfw_op_lock.release();
    };

    auto* ctx = _ctx.exchange(nullptr);
    if (ctx == nullptr)
    {
        // window has stopped already
        return boole::False;
    }
    glfwDestroyWindow(ctx);
    if (--_window_number == 0)
    {
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