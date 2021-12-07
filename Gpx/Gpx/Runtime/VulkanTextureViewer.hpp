#pragma once

#include "Interface.hpp"

namespace gpx
{

class vulkan_runtime;

class vulkan_texture_viewer : public texture_viewer
{
    friend class vulkan_pipeline;

public:
    vulkan_texture_viewer(const texture_viewer_desc& desc, obs<vulkan_runtime> rt);

    virtual ~vulkan_texture_viewer();

    virtual boole load() override;

    virtual boole unload() override;

private:
    texture_viewer_desc      _desc;
    obs<vulkan_runtime>      _rt;
    VkImage                  _vk_image;
    VkDeviceMemory           _vk_memory;
    VkImageView              _vk_image_view;
    VkSampler                _vk_sampler;
};

}
