#pragma once

#include "Interface.hpp"
#include "DynamicMemory.hpp"

namespace gpx
{

class vulkan_runtime;

class vulkan_pipeline;

class __vulkan_dynamic_memory_mem
{
public:
    __vulkan_dynamic_memory_mem() = delete;

    __vulkan_dynamic_memory_mem(s64 sz) :
        _mem(memory::alloc<void>(sz)),
        _guid(guid::new_instance())
    {
    }

    ~__vulkan_dynamic_memory_mem()
    {
        memory::free(_mem);
    }

public:
    void* _mem;
    guid  _guid;
};

class vulkan_dynamic_memory : public dynamic_memory
{
public:
    vulkan_dynamic_memory(obs<vulkan_runtime> rt);

    virtual ~vulkan_dynamic_memory() override;

    virtual s64 memory_size() override;

    virtual boole update(void* src) override;

public:
    boole init(const dynamic_memory_desc& desc, obs<vulkan_dynamic_memory> ob_dm);

    boole uninit();

public:
    boole add_pipeline_memory(obs<vulkan_pipeline> pipeline, VkDeviceMemory memory);

    VkDeviceMemory get_pipeline_memory(obs<vulkan_pipeline> pipeline, s64 idx);

    boole update_pipeline_memory(obs<vulkan_pipeline> pipeline, s64 idx);

    boole remove_pipeline_memory(obs<vulkan_pipeline> pipeline);

private:
    dynamic_memory_desc           _desc;
    obs<vulkan_runtime>           _rt;
    obs<vulkan_dynamic_memory>    _self;

    ref<__vulkan_dynamic_memory_mem>
                                  _mem;

    map<
        obs<vulkan_pipeline>, vector<VkDeviceMemory>
    >                             _pipeline_memory_map;
    rw_lock                       _pipeline_memory_map_lock;
};

}
