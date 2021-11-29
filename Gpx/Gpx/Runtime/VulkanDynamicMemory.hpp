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

private:
    dynamic_memory_desc            _desc;
    obs<vulkan_runtime>            _rt;

    VkBuffer                       _buffer;
    VkDeviceMemory                 _memory;
};

}
