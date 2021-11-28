#pragma once

#include "Interface.hpp"

namespace gpx
{

class vulkan_runtime;
struct vulkan_window_context;

class vulkan_pipeline : public pipeline
{
public:
    vulkan_pipeline(obs<vulkan_runtime> rt, obs<vulkan_window_context> w_ctx);

    virtual ~vulkan_pipeline() override;

    virtual boole init(const pipeline_desc& desc) override;

    virtual boole uninit() override;

    virtual boole setup_vertices_buffer(ref<vertices_buffer> vertices_buffer) override;

    virtual boole clear_vertices_buffer() override;

    virtual boole load_resource() override;

    virtual boole unload_resource() override;

    virtual boole render() override;

private:
    boole setup_vk_buffer(
        sz_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    boole copy_vk_buffer(const void* data, VkBuffer device_buf, sz_t size);

    s64 get_vk_memory_type(VkMemoryRequirements& requirements, VkFlags needed_properties);

private:
    obs<vulkan_runtime>         _rt;
    obs<vulkan_window_context>  _window_ctx;
    VkPipelineLayout            _layout;
    VkRenderPass                _render_pass;
    VkPipeline                  _pipeline;
    vector<VkFramebuffer>       _frame_buffer_vec;
    vector<VkCommandBuffer>     _command_buffer_vec;

    vector<
        ref<vertices_buffer>
    >                           _vertices_buffers;
    vector<VkBuffer>            _vk_vertices_buffers;
    vector<VkDeviceMemory>      _vk_vertices_memories;
};

}
