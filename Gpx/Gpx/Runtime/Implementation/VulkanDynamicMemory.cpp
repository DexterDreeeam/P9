
#include "../VulkanDynamicMemory.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_dynamic_memory::vulkan_dynamic_memory(obs<vulkan_runtime> rt) :
    dynamic_memory(),
    _desc(),
    _rt(rt),
    _descriptor_set_update_map(),
    _descriptor_set_update_map_lock(),
    _buffer(),
    _memory(),
    _buffer_info(),
    _buffer_in_use(0),
    _update_lock()
{
    AUTO_TRACE;
    _descriptor_set_update_map_lock.init();
    _update_lock.init();
}

vulkan_dynamic_memory::~vulkan_dynamic_memory()
{
    AUTO_TRACE;

    uninit();
    _update_lock.uninit();
    _descriptor_set_update_map_lock.uninit();
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

    VkBuffer buffer[2];
    VkDeviceMemory memory[2];

    if (!vulkan_runtime::setup_vk_buffer(
            logical_device,
            rt->get_vk_physical_device(),
            desc._size,
            rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            buffer[0],
            memory[0]))
    {
        uninit();
        return boole::False;
    }

    if (!vulkan_runtime::setup_vk_buffer(
            logical_device,
            rt->get_vk_physical_device(),
            desc._size,
            rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            buffer[1],
            memory[1]))
    {
        uninit();
        return boole::False;
    }

    _buffer[0] = buffer[0];
    _memory[0] = memory[0];
    _buffer[1] = buffer[1];
    _memory[1] = memory[1];

    _buffer_info[0].buffer = buffer[0];
    _buffer_info[0].offset = 0;
    _buffer_info[0].range = desc._size;
    _buffer_info[1].buffer = buffer[1];
    _buffer_info[1].offset = 0;
    _buffer_info[1].range = desc._size;

    _buffer_in_use = 0;

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

    if (_buffer[0])
    {
        if (!vulkan_runtime::clear_vk_buffer(
                rt->get_vk_logical_device(),
                _buffer[0],
                _memory[0]))
        {
            return boole::False;
        }
        _buffer[0] = nullptr;
        _memory[0] = nullptr;
    }

    if (_buffer[1])
    {
        if (!vulkan_runtime::clear_vk_buffer(
                rt->get_vk_logical_device(),
                _buffer[1],
                _memory[1]))
        {
            return boole::False;
        }
        _buffer[1] = nullptr;
        _memory[1] = nullptr;
    }
    return boole::True;
}

boole vulkan_dynamic_memory::update(void* src)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }
    auto logical_device = rt->get_vk_logical_device();

    if (!_update_lock.wait_acquire())
    {
        return boole::False;
    }

    escape_function ef =
        [=]() mutable
        {
            _update_lock.release();
        };

    u64 update_idx = (_buffer_in_use + 1) % 2;
    void* mmap;
    if (!vkMapMemory(logical_device, _memory[update_idx], 0, _desc._size, 0, &mmap))
    {
        return boole::False;
    }
    memory::copy(src, mmap, _desc._size);
    vkUnmapMemory(logical_device, _memory[update_idx]);

    for (auto& p : _descriptor_set_update_map)
    {
        p.second.pBufferInfo = &_buffer_info[update_idx];
        vkUpdateDescriptorSets(rt->get_vk_logical_device(), 1, &p.second, 0, nullptr);
    }

    _buffer_in_use = update_idx;
    return boole::True;
}

boole vulkan_dynamic_memory::add_pipeline_descriptor_set(const string& pipeline_name, const VkWriteDescriptorSet& ds)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    if (!_descriptor_set_update_map_lock.wait_acquire())
    {
        return boole::False;
    }

    escape_function ef =
        [=]() mutable
        {
            _descriptor_set_update_map_lock.release();
        };

    _descriptor_set_update_map[pipeline_name] = ds;
    auto& myds = _descriptor_set_update_map[pipeline_name];
    myds.pBufferInfo = &_buffer_info[_buffer_in_use];
    vkUpdateDescriptorSets(rt->get_vk_logical_device(), 1, &myds, 0, nullptr);

    return boole::True;
}

boole vulkan_dynamic_memory::remove_pipeline_descriptor_set(const string& pipeline_name)
{
    AUTO_TRACE;

    if (!_descriptor_set_update_map_lock.wait_acquire())
    {
        return boole::False;
    }

    escape_function ef =
        [=]() mutable
        {
            _descriptor_set_update_map_lock.release();
        };

    return _descriptor_set_update_map.erase(pipeline_name);
}

}
