#pragma once

#include "Interface.hpp"

namespace gpx
{

struct vulkan_window_context;

struct vulkan_pipeline : public pipeline
{
    obs<vulkan_window_context>  _window_ctx;
};

}
