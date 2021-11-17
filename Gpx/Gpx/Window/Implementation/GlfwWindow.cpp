
#define VK_USE_PLATFORM_WIN32_KHR
#include "../../Runtime/VulkanRuntime.hpp"
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
    AUTO_TRACE;

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

    if (_ctx != nullptr)
    {
        return boole::False;
    }

    ++_window_number;

    assert(glfwVulkanSupported() == GLFW_TRUE);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto* ctx = glfwCreateWindow(
        _desc.width, _desc.height, _desc.name.data(), nullptr, nullptr);

    if (ctx == nullptr)
    {
        return boole::False;
    }

    auto rt = _rt.try_ref();
    assert(rt.has_value());

    VkSurfaceKHR surface;
    VkInstance vk = rt.ref_of<vulkan_runtime>()->get_vk_instance();
    if (glfwCreateWindowSurface(vk, ctx, nullptr, &surface) != VK_SUCCESS)
    {
        goto L_error;
    }
    _ctx = ctx;
    _surface = surface;
    if (rt.ref_of<vulkan_runtime>()->window_start_callback(_desc.name) == boole::False)
    {
        goto L_error;
    }

    return boole::True;

L_error:
    if (surface)
    {
        vkDestroySurfaceKHR(rt.ref_of<vulkan_runtime>()->get_vk_instance(), surface, nullptr);
        _surface = nullptr;
    }
    if (ctx)
    {
        glfwDestroyWindow(ctx);
        _ctx = nullptr;
    }
    --_window_number;
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

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }
    if (rt.ref_of<vulkan_runtime>()->window_stop_callback(_desc.name) == boole::False)
    {
        return boole::False;
    }
    if (_surface)
    {
        vkDestroySurfaceKHR(rt.ref_of<vulkan_runtime>()->get_vk_instance(), _surface, nullptr);
        _surface = nullptr;
    }
    if (_ctx)
    {
        glfwDestroyWindow(_ctx);
        _ctx = nullptr;
    }
    --_window_number;
    return boole::True;
}

boole glfw_window::present(s64 my_image)
{
    AUTO_TRACE;

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
    AUTO_TRACE;

    return _ctx != nullptr;
}

}