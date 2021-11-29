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

    virtual ~vulkan_vertices_viewer();

    virtual boole load() override;

    virtual boole unload() override;

private:
    vertices_viewer_desc     _desc;
    obs<vulkan_runtime>      _rt;
    VkBuffer                 _vk_buffer;
    VkDeviceMemory           _vk_memory;
    VkBuffer                 _vk_buffer_indices;
    VkDeviceMemory           _vk_memory_indices;
    vertices_buffer_header   _vb_header;
};

}
