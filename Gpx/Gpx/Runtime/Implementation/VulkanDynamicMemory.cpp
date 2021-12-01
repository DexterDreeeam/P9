
#include "../VulkanDynamicMemory.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_dynamic_memory::vulkan_dynamic_memory(obs<vulkan_runtime> rt) :
    dynamic_memory(),
    _desc(),
    _rt(rt),
    _self(),
    _mem(),
    _pipeline_memory_map(),
    _pipeline_memory_map_lock()
{
    AUTO_TRACE;
    _pipeline_memory_map_lock.init();
}

vulkan_dynamic_memory::~vulkan_dynamic_memory()
{
    AUTO_TRACE;

    uninit();
    _pipeline_memory_map_lock.uninit();
}

boole vulkan_dynamic_memory::init(const dynamic_memory_desc& desc, obs<vulkan_dynamic_memory> ob_dm)
{
    AUTO_TRACE;

    _self = ob_dm;
    _mem = ref<__vulkan_dynamic_memory_mem>::new_instance(desc._size);
    _desc = desc;
    return boole::True;
}

boole vulkan_dynamic_memory::uninit()
{
    AUTO_TRACE;

    _self = obs<vulkan_dynamic_memory>();
    _mem = ref<__vulkan_dynamic_memory_mem>();
    return boole::True;
}

s64 vulkan_dynamic_memory::memory_size()
{
    return _desc._size;
}

boole vulkan_dynamic_memory::update(void* src)
{
    AUTO_TRACE;

    auto new_mem = ref<__vulkan_dynamic_memory_mem>::new_instance(_desc._size);
    memory::copy(src, new_mem->_mem, _desc._size);
    _mem = new_mem;

    if (!_pipeline_memory_map_lock.wait_read())
    {
        return boole::False;
    }

    escape_function ef_lock_release =
        [=]() mutable
        {
            _pipeline_memory_map_lock.read_release();
        };

    for (auto& p : _pipeline_memory_map)
    {
        auto ob = p.first;
        auto rf = ob.try_ref();
        if (rf.has_value())
        {
            rf->alert_dynamic_memory_change(_self);
        }
    }

    return boole::True;
}

boole vulkan_dynamic_memory::add_pipeline_memory(obs<vulkan_pipeline> pipeline, VkDeviceMemory memory)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    if (!_pipeline_memory_map_lock.wait_write())
    {
        return boole::False;
    }

    escape_function ef =
        [=]() mutable
        {
            _pipeline_memory_map_lock.write_release();
        };

    _pipeline_memory_map[pipeline].push_back(memory);
    return boole::True;
}

VkDeviceMemory vulkan_dynamic_memory::get_pipeline_memory(obs<vulkan_pipeline> pipeline, s64 idx)
{
    if (!_pipeline_memory_map_lock.wait_read())
    {
        return nullptr;
    }

    escape_function ef =
        [=]() mutable
        {
            _pipeline_memory_map_lock.read_release();
        };

    auto itr = _pipeline_memory_map.find(pipeline);
    if (itr == _pipeline_memory_map.end())
    {
        return nullptr;
    }
    assert(idx < itr->second.size());
    return itr->second[idx];
}

boole vulkan_dynamic_memory::update_pipeline_memory(obs<vulkan_pipeline> pipeline, s64 idx)
{
    auto memory = get_pipeline_memory(pipeline, idx);
    if (!memory)
    {
        return boole::False;
    }
    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    void* mmap;
    if (vkMapMemory(logical_device, memory, 0, _desc._size, 0, &mmap) != VK_SUCCESS)
    {
        return boole::False;
    }
    auto m = _mem;
    memory::copy(m->_mem, mmap, _desc._size);
    vkUnmapMemory(logical_device, memory);
    return boole::True;
}

boole vulkan_dynamic_memory::remove_pipeline_memory(obs<vulkan_pipeline> pipeline)
{
    AUTO_TRACE;

    if (!_pipeline_memory_map_lock.wait_write())
    {
        return boole::False;
    }

    escape_function ef =
        [=]() mutable
        {
            _pipeline_memory_map_lock.write_release();
        };

    return _pipeline_memory_map.erase(pipeline);
}

}
