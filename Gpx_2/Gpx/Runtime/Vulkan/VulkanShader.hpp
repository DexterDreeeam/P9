#pragma once

namespace gpx
{

class vulkan_shader : public shader
{
    friend class vulkan_pipeline;

public:
    vulkan_shader(const shader_desc& desc, obs<runtime> rt);

    virtual ~vulkan_shader() override;

    virtual boole load(const string& shader_path) override;

    virtual boole unload() override;

    virtual shader_type type() override;

private:
    shader_desc                 _desc;
    obs<runtime>                _rt;
    VkShaderModule              _shader_module;
};

}
