#pragma once

#include "../../../External/Vulkan/Windows/Header/glfw3.h"

#include "../Interface.hpp"

namespace gpx
{

class glfw_window : public window
{
public:
    glfw_window(const window_desc& desc);

    virtual ~glfw_window() override;

public:
    virtual boole start() override;

    virtual boole stop() override;

    virtual boole present(s64 my_image) override;

    virtual boole poll_event() override;

    virtual boole is_running() override;

private:
    window_desc        _desc;
    atom<GLFWwindow*>  _ctx;
};

}
