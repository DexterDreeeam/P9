
#include "../Interface.hpp"
#include "../VulkanInterface.hpp"

namespace gpx
{

vulkan_texture_viewer::vulkan_texture_viewer(const texture_viewer_desc& desc, obs<vulkan_runtime> rt) :
    _desc(desc),
    _rt(rt),
    _vk_image(nullptr),
    _vk_memory(nullptr),
    _vk_image_view(nullptr),
    _vk_sampler(nullptr)
{
}

vulkan_texture_viewer::~vulkan_texture_viewer()
{
    assert(state() == texture_viewer_state::Offline);
}

boole vulkan_texture_viewer::load()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    if (!transfer_state(texture_viewer_state::Offline, texture_viewer_state::Transition))
    {
        return boole::False;
    }
    escape_function ef_error_transfer_offline =
        [=]() mutable
        {
            transfer_state(texture_viewer_state::Transition, texture_viewer_state::Offline);
        };

    VkBuffer host_buffer = nullptr;
    VkDeviceMemory host_memory = nullptr;
    escape_function ef_release_host_mem =
        [&]() mutable
        {
            vulkan_runtime::clear_vk_buffer(logical_device, host_buffer, host_memory);
        };

    void* data;
    auto tt = texture::load_with_allocator(
        _desc._file_path,
        [&](sz_t sz) mutable -> void*
        {
            if (!vulkan_runtime::setup_vk_buffer(
                logical_device,
                rt->get_vk_physical_device(),
                sz,
                rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                host_buffer,
                host_memory))
            {
                return nullptr;
            }

            void* mmap = nullptr;
            vkMapMemory(logical_device, host_memory, 0, sz, 0, &mmap);
            data = mmap;
            return mmap;
        });

    if (!host_buffer || !host_memory)
    {
        return boole::False;
    }

    vkUnmapMemory(logical_device, host_memory);

    s64 mipmap_level = 1;
    if (_desc._mipmap_enable)
    {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(rt->get_vk_physical_device(), VK_FORMAT_R8G8B8A8_SRGB, &formatProperties);
        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)
        {
            s64 extent = math::min(tt.width, tt.height);
            while (1)
            {
                extent /= vulkan_runtime::mipmap_base_factor;
                if (extent < vulkan_runtime::mipmap_base_factor)
                {
                    break;
                }
                ++mipmap_level;
            }
        }
    }

    VkImage device_image = nullptr;
    VkDeviceMemory device_image_memory = nullptr;
    VkImageView device_image_view = nullptr;
    VkSampler sampler = nullptr;
    escape_function ef_error_release_image =
        [&]() mutable
        {
            if (sampler)
            {
                vkDestroySampler(logical_device, sampler, nullptr);
            }
            if (device_image_view)
            {
                vkDestroyImageView(logical_device, device_image_view, nullptr);
            }
            if (device_image)
            {
                vulkan_runtime::clear_vk_image(logical_device, device_image, device_image_memory);
            }
        };

    if (!vulkan_runtime::setup_vk_texture_image(
        logical_device,
        rt->get_vk_physical_device(),
        tt.width, tt.height, mipmap_level,
        device_image, device_image_memory))
    {
        return boole::False;
    }

    if (!vulkan_runtime::convert_vk_image_layout(
        logical_device, rt->_transfer_command_pool, rt->_transfer_queue,
        device_image, 1, mipmap_level, tt.width, tt.height))
    {
        return boole::False;
    }

    if (!vulkan_runtime::copy_vk_buffer_to_texture_image(
        logical_device, rt->_transfer_command_pool, rt->_transfer_queue,
        host_buffer, 0, device_image, tt.width, tt.height))
    {
        return boole::False;
    }

    if (!vulkan_runtime::convert_vk_image_layout(
        logical_device, rt->_transfer_command_pool, rt->_transfer_queue,
        device_image, 2, mipmap_level, tt.width, tt.height))
    {
        return boole::False;
    }

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = device_image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = mipmap_level;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    if (vkCreateImageView(logical_device, &createInfo, nullptr, &device_image_view) != VK_SUCCESS)
    {
        return boole::False;
    }

    //VkPhysicalDeviceProperties physical_properties = {};
    //vkGetPhysicalDeviceProperties(rt->get_vk_physical_device(), &physical_properties);

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f; //physical_properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = mipmap_level;
    samplerInfo.mipLodBias = 0;

    if (vkCreateSampler(logical_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
    {
        return boole::False;
    }

    _vk_image = device_image;
    _vk_memory = device_image_memory;
    _vk_image_view = device_image_view;
    _vk_sampler = sampler;
    ef_error_transfer_offline.disable();
    ef_error_release_image.disable();
    transfer_state(texture_viewer_state::Transition, texture_viewer_state::Online);
    return boole::True;
}

boole vulkan_texture_viewer::unload()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        boole::False;
    }

    if (!transfer_state(texture_viewer_state::Online, texture_viewer_state::Transition))
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    if (_vk_sampler)
    {
        vkDestroySampler(logical_device, _vk_sampler, nullptr);
        _vk_sampler = nullptr;
    }
    if (_vk_image_view)
    {
        vkDestroyImageView(logical_device, _vk_image_view, nullptr);
        _vk_image_view = nullptr;
    }
    if (_vk_image && _vk_memory)
    {
        vulkan_runtime::clear_vk_image(logical_device, _vk_image, _vk_memory);
        _vk_image = nullptr;
        _vk_memory = nullptr;
    }

    boole checker = transfer_state(texture_viewer_state::Transition, texture_viewer_state::Offline);
    assert(checker);
    return boole::True;
}

}
