
#include "../VulkanDynamicMemory.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_dynamic_memory::vulkan_dynamic_memory(obs<vulkan_runtime> rt) :
    dynamic_memory(),
    _desc(),
    _rt(rt),
    _buffer_vec(),
    _memory_vec()
{
    AUTO_TRACE;
}

vulkan_dynamic_memory::~vulkan_dynamic_memory()
{
    AUTO_TRACE;

    uninit();
}

boole vulkan_dynamic_memory::init(const dynamic_memory_desc& desc)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();
    s64 image_count = rt->_desc.frame_count;

    for (s64 i = 0; i < image_count; ++i)
    {
        VkBuffer buffer;
        VkDeviceMemory memory;
        if (!vulkan_runtime::setup_vk_buffer(
                logical_device,
                rt->get_vk_physical_device(),
                desc._size,
                rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                buffer,
                memory))
        {
            uninit();
            return boole::False;
        }
        _buffer_vec.push_back(buffer);
        _memory_vec.push_back(memory);
    }

    _desc = desc;
    return boole::True;
}

boole vulkan_dynamic_memory::uninit()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    if (_buffer)
    {
        if (!vulkan_runtime::clear_vk_buffer(
            rt->get_vk_logical_device(),
            _buffer,
            _memory))
        {
            return boole::False;
        }
    }
    _buffer = nullptr;
    _memory = nullptr;
    return boole::True;
}

boole vulkan_dynamic_memory::update(void* src)
{
    return boole::True;
}

}
