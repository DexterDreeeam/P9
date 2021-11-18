#pragma once

#include "Interface.hpp"

namespace gpx
{

struct vulkan_window_context;

class vulkan_shader : public shader
{
public:
    vulkan_shader(const shader_desc& desc, obs<vulkan_window_context> w_ctx);

    virtual ~vulkan_shader() override;

    virtual boole load(const string& shader_path) override;

private:
    boole unload();

private:
    shader_desc                 _desc;
    obs<vulkan_window_context>  _window_ctx;
    VkShaderModule              _shader_module;
};

}
