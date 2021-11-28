
#include "../VulkanRuntime.hpp"
#include "../VulkanPipeline.hpp"
#include "../VulkanShader.hpp"

namespace gpx
{

vulkan_shader::vulkan_shader(const shader_desc& desc, obs<runtime> rt) :
    _desc(desc),
    _rt(rt),
    _shader_module(nullptr)
{
}

vulkan_shader::~vulkan_shader()
{
    unload();
}

boole vulkan_shader::load(const string& shader_path)
{
    auto rt = _rt.try_ref().ref_of<vulkan_runtime>();
    if (rt.empty())
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
    if (vkCreateShaderModule(rt->get_vk_logical_device(), &createInfo, nullptr, &_shader_module) != VK_SUCCESS)
    {
        return boole::False;
    }

    return boole::True;
}

boole vulkan_shader::unload()
{
    auto rt = _rt.try_ref().ref_of<vulkan_runtime>();
    if (rt.empty())
    {
        return boole::False;
    }
    auto shader_module = _shader_module;
    if (shader_module == nullptr)
    {
        return boole::False;
    }
    vkDestroyShaderModule(rt->get_vk_logical_device(), shader_module, nullptr);
    _shader_module = nullptr;
    return boole::True;
}

shader_type vulkan_shader::type()
{
    return _desc._type;
}

}
