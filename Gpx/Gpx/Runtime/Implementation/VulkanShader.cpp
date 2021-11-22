
#include "../VulkanRuntime.hpp"
#include "../VulkanPipeline.hpp"
#include "../VulkanShader.hpp"

namespace gpx
{

vulkan_shader::vulkan_shader(const shader_desc& desc, obs<vulkan_window_context> w_ctx) :
    _desc(desc),
    _window_ctx(w_ctx),
    _shader_module(nullptr)
{
}

vulkan_shader::~vulkan_shader()
{
    unload();
}

boole vulkan_shader::load(const string& shader_path)
{
    auto w_ctx = _window_ctx.try_ref().ref_of<vulkan_window_context>();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    VkShaderModuleCreateInfo createInfo = {};
    file f;
    char buf[math::kilobyte() * 64];
    s64 f_len;
    if (!f.init_input(shader_path.data()))
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
        {
            if (f.is_init())
            {
                f.uninit();
            }
        };

    if (!f.input(buf, math::kilobyte() * 64, f_len))
    {
        return boole::False;
    }

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = (size_t)f_len;
    createInfo.pCode = pointer_convert(buf, 0, uint32_t*);
    if (vkCreateShaderModule(w_ctx->_logical_device, &createInfo, nullptr, &_shader_module) != VK_SUCCESS)
    {
        return boole::False;
    }

    return boole::True;
}

boole vulkan_shader::unload()
{
    auto w_ctx = _window_ctx.try_ref().ref_of<vulkan_window_context>();
    if (w_ctx.empty())
    {
        return boole::False;
    }
    auto shader_module = _shader_module;
    if (shader_module == nullptr)
    {
        return boole::False;
    }
    vkDestroyShaderModule(w_ctx->_logical_device, shader_module, nullptr);
    _shader_module = nullptr;
    return boole::True;
}

shader_type vulkan_shader::type()
{
    return _desc._type;
}

}
