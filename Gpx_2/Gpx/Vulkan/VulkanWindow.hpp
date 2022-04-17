#pragma once

#include "_Vulkan.hpp"
#include "_Declaration.hpp"

namespace gpx
{

class vulkan_window : public window
{
    friend class vulkan_runtime;

public:
    vulkan_window(const window_desc& desc, obs<runtime> rt);

    virtual ~vulkan_window() override;

public:
    virtual string name() override;

    virtual boole start() override;

    virtual boole stop() override;

    // virtual boole present(s64 my_image) override;

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
