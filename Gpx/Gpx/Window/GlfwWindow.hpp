#pragma once

#define GLFW_INCLUDE_VULKAN
#include "../../External/Vulkan/Windows/Header/glfw3.h"
#include "../Runtime/Interface.hpp"

namespace gpx
{

class runtime;

class glfw_window : public window
{
    friend class vulkan_runtime;

public:
    glfw_window(const window_desc& desc, obs<runtime> rt);

    virtual ~glfw_window() override;

public:
    virtual string name() override;

    virtual boole start() override;

    virtual boole stop() override;

    virtual boole present(s64 my_image) override;

    virtual boole poll_event() override;

    virtual boole is_running() override;

private:
    static mutex       _glfw_op_lock;
    static s64         _window_number;

    window_desc        _desc;
    obs<runtime>       _rt;

    GLFWwindow*        _ctx;
    VkSurfaceKHR       _surface;
};

}
