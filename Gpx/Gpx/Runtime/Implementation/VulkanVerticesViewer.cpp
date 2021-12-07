
#include "../Interface.hpp"
#include "../VulkanInterface.hpp"

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
        transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Offline);
        return boole::False;
    }

    vkUnmapMemory(logical_device, host_memory);

    escape_function ef_release_host_buffer =
        [=]() mutable
        {
            vulkan_runtime::clear_vk_buffer(logical_device, host_buffer, host_memory);
        };

    boole rst = boole::False;

    if (!vulkan_runtime::setup_vk_buffer(
            logical_device,
            rt->get_vk_physical_device(),
            vb->vertices_data_size(),
            rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _vk_buffer, _vk_memory))
    {
        goto L_finish;
    }

    if (!vulkan_runtime::copy_vk_buffer(
            logical_device, rt->_transfer_command_pool, rt->_transfer_queue,
            host_buffer, 0, _vk_buffer, vb->vertices_data_size()))
    {
        goto L_finish;
    }

    if (vb->vertex_indices_data_size())
    {
        if (!vulkan_runtime::setup_vk_buffer(
                logical_device,
                rt->get_vk_physical_device(),
                vb->vertex_indices_data_size(),
                rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                _vk_buffer_indices, _vk_memory_indices))
        {
            goto L_finish;
        }

        if (!vulkan_runtime::copy_vk_buffer(
                logical_device, rt->_transfer_command_pool, rt->_transfer_queue,
                host_buffer, vb->vertices_data_size(), _vk_buffer_indices, vb->vertex_indices_data_size()))
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
            vulkan_runtime::clear_vk_buffer(logical_device, _vk_buffer, _vk_memory);
            _vk_buffer = nullptr;
            _vk_memory = nullptr;
        }
        if (_vk_buffer_indices)
        {
            vulkan_runtime::clear_vk_buffer(logical_device, _vk_buffer_indices, _vk_memory_indices);
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

    if (_vk_buffer && _vk_memory)
    {
        vulkan_runtime::clear_vk_buffer(logical_device, _vk_buffer, _vk_memory);
        _vk_buffer = nullptr;
        _vk_memory = nullptr;
    }
    if (_vk_buffer_indices && _vk_memory_indices)
    {
        vulkan_runtime::clear_vk_buffer(logical_device, _vk_buffer_indices, _vk_memory_indices);
        _vk_buffer_indices = nullptr;
        _vk_memory_indices = nullptr;
    }

    boole checker = transfer_state(vertices_viewer_state::Transition, vertices_viewer_state::Offline);
    assert(checker);
    return boole::True;
}

}
