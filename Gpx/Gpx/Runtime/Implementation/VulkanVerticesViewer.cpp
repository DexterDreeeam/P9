
#include "../Interface.hpp"
#include "../VulkanRuntime.hpp"
#include "../VulkanVerticesViewer.hpp"

namespace gpx
{

vulkan_vertices_viewer::vulkan_vertices_viewer(const vertices_viewer_desc& desc, obs<vulkan_runtime> rt) :
    _desc(desc),
    _rt(rt),
    _vk_buffer(nullptr),
    _vk_memory(nullptr),
    _vk_buffer_indices(nullptr),
    _vk_memory_indices(nullptr),
    _vb_header()
{
}

vulkan_vertices_viewer::~vulkan_vertices_viewer()
{
    assert(state() == vertices_viewer_state::Offline);
}

boole vulkan_vertices_viewer::load()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        boole::False;
    }

    if (!transfer_state(vertices_viewer_state::Offline, vertices_viewer_state::Transition))
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    VkBuffer host_buffer = nullptr;
    VkDeviceMemory host_memory = nullptr;
    void* data;
    auto vb = vertices_buffer::load_with_allocator(
        _desc._file_path,
        [&](sz_t sz) mutable -> void*
        {
            if (!setup_vk_buffer(
                sz,
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
        transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Offline);
        return boole::False;
    }

    vkUnmapMemory(logical_device, host_memory);

    escape_function ef_release_host_buffer =
        [=]() mutable
        {
            vkDestroyBuffer(logical_device, host_buffer, nullptr);
            vkFreeMemory(logical_device, host_memory, nullptr);
        };

    boole rst = boole::False;

    if (!setup_vk_buffer(
        vb->vertices_data_size(),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        _vk_buffer, _vk_memory))
    {
        goto L_finish;
    }

    if (!copy_vk_buffer(host_buffer, 0, _vk_buffer, vb->vertices_data_size()))
    {
        goto L_finish;
    }

    if (vb->vertex_indices_data_size())
    {
        if (!setup_vk_buffer(
            vb->vertex_indices_data_size(),
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _vk_buffer_indices, _vk_memory_indices))
        {
            goto L_finish;
        }

        if (!copy_vk_buffer(host_buffer, vb->vertices_data_size(), _vk_buffer_indices, vb->vertex_indices_data_size()))
        {
            goto L_finish;
        }
    }

    _vb_header.vertex_type = vb->vertices_type();
    _vb_header.vertices_count = vb->vertices_count();
    _vb_header.vertices_data_size = vb->vertices_data_size();
    _vb_header.indices_count = vb->vertex_indices_count();
    _vb_header.indices_data_size = vb->vertex_indices_data_size();
    rst = boole::True;

L_finish:
    if (rst)
    {
        transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Online);
    }
    else
    {
        if (_vk_buffer)
        {
            clear_vk_buffer(_vk_buffer, _vk_memory);
            _vk_buffer = nullptr;
            _vk_memory = nullptr;
        }
        if (_vk_buffer_indices)
        {
            clear_vk_buffer(_vk_buffer_indices, _vk_memory_indices);
            _vk_buffer_indices = nullptr;
            _vk_memory_indices = nullptr;
        }
        transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Offline);
    }
    return rst;
}

boole vulkan_vertices_viewer::unload()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        boole::False;
    }

    if (!transfer_state(vertices_viewer_state::Online, vertices_viewer_state::Transition))
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    if (_vk_buffer)
    {
        vkDestroyBuffer(logical_device, _vk_buffer, nullptr);
        _vk_buffer = nullptr;
    }
    if (_vk_memory)
    {
        vkFreeMemory(logical_device, _vk_memory, nullptr);
        _vk_memory = nullptr;
    }
    if (_vk_buffer_indices)
    {
        vkDestroyBuffer(logical_device, _vk_buffer_indices, nullptr);
        _vk_buffer_indices = nullptr;
    }
    if (_vk_memory_indices)
    {
        vkFreeMemory(logical_device, _vk_memory_indices, nullptr);
        _vk_memory_indices = nullptr;
    }

    boole checker = transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Offline);
    assert(checker);
    return boole::True;
}

boole vulkan_vertices_viewer::setup_vk_buffer(
    sz_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();
    VkBuffer vk_buffer;
    VkDeviceMemory vk_memory;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode =
        rt->_render_queue_family_idx == rt->_transfer_queue_family_idx ?
        VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;

    if (vkCreateBuffer(logical_device, &bufferInfo, nullptr, &vk_buffer) != VK_SUCCESS)
    {
        return boole::False;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logical_device, vk_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = get_vk_memory_type(memRequirements, properties);

    if (vkAllocateMemory(logical_device, &allocInfo, nullptr, &vk_memory) != VK_SUCCESS)
    {
        vkDestroyBuffer(logical_device, vk_buffer, nullptr);
        return boole::False;
    }

    if (vkBindBufferMemory(logical_device, vk_buffer, vk_memory, 0) != VK_SUCCESS)
    {
        vkDestroyBuffer(logical_device, vk_buffer, nullptr);
        vkFreeMemory(logical_device, vk_memory, nullptr);
        return boole::False;
    }

    buffer = vk_buffer;
    bufferMemory = vk_memory;
    return boole::True;
}

boole vulkan_vertices_viewer::clear_vk_buffer(VkBuffer buffer, VkDeviceMemory bufferMemory)
{
    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }
    auto logical_device = rt->get_vk_logical_device();
    vkDestroyBuffer(logical_device, buffer, nullptr);
    vkFreeMemory(logical_device, bufferMemory, nullptr);
    return boole::True;
}

boole vulkan_vertices_viewer::copy_vk_buffer(VkBuffer host_buf, s64 host_offset, VkBuffer device_buf, sz_t size)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }
    auto logical_device = rt->_logical_device;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = rt->_transfer_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        return boole::False;
    }
    escape_function ef_release_command_buffer =
        [=]() mutable
    {
        vkFreeCommandBuffers(logical_device, rt->_transfer_command_pool, 1, &commandBuffer);
    };

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = host_offset;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    vkCmdCopyBuffer(commandBuffer, host_buf, device_buf, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);
    vkQueueSubmit(rt->_transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(rt->_transfer_queue);
    return boole::True;
}

s64 vulkan_vertices_viewer::get_vk_memory_type(VkMemoryRequirements& requirements, VkFlags needed_properties)
{
    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return -1;
    }
    auto typeBits = requirements.memoryTypeBits;
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(rt->_physical_device, &memProperties);
    for (s64 i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if ((typeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & needed_properties) == needed_properties)
        {
            return i;
        }
    }
    return -1;
}

}
