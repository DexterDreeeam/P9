#pragma once

#include "Interface.hpp"

namespace gpx
{

class vulkan_vertices_viewer : public vertices_viewer
{
public:
    vulkan_vertices_viewer() = default;

    virtual ~vulkan_vertices_viewer() = default;

    virtual boole init(const string& path, obs<runtime> rt) override;

    virtual boole uninit() override;

    virtual boole load() override;

    virtual boole unload() override;

private:
    VkBuffer         _vk_buffer;
    VkDeviceMemory   _vk_memory;
};

}
