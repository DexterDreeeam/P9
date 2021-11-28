#pragma once

#include "Interface.hpp"

namespace gpx
{

class vulkan_runtime;

class vulkan_vertices_viewer : public vertices_viewer
{
    friend class vulkan_pipeline;

public:
    vulkan_vertices_viewer(const vertices_viewer_desc& desc, obs<vulkan_runtime> rt);

    virtual ~vulkan_vertices_viewer() = default;

    virtual boole load() override;

    virtual boole unload() override;

private:
    boole setup_vk_buffer(
        sz_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    boole clear_vk_buffer(VkBuffer buffer, VkDeviceMemory bufferMemory);

    boole copy_vk_buffer(VkBuffer host_buf, VkBuffer device_buf, sz_t size);

    s64 get_vk_memory_type(VkMemoryRequirements& requirements, VkFlags needed_properties);

private:
    vertices_viewer_desc     _desc;
    obs<vulkan_runtime>      _rt;
    VkBuffer                 _vk_buffer;
    VkDeviceMemory           _vk_memory;
    vertices_buffer_header   _vb_header;
};

}
