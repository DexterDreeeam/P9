#pragma once

#include "Interface.hpp"
#include "DynamicMemory.hpp"

namespace gpx
{

class vulkan_runtime;

class vulkan_dynamic_memory : public dynamic_memory
{
public:
    vulkan_dynamic_memory(obs<vulkan_runtime> rt);

    virtual ~vulkan_dynamic_memory() override;

    virtual boole update(void* src) override;

public:
    boole init(const dynamic_memory_desc& desc);

    boole uninit();

public:
    boole add_pipeline_descriptor_set(const string& pipeline_name, const VkWriteDescriptorSet& ds);

    boole remove_pipeline_descriptor_set(const string& pipeline_name);

private:
    dynamic_memory_desc           _desc;
    obs<vulkan_runtime>           _rt;

    map<
        string, VkWriteDescriptorSet
    >                             _descriptor_set_update_map;
    mutex                         _descriptor_set_update_map_lock;

    VkBuffer                      _buffer[2];
    VkDeviceMemory                _memory[2];
    VkDescriptorBufferInfo        _buffer_info[2];
    u64                           _buffer_in_use;

    mutex                         _update_lock;
};

}
