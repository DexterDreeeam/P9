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

    virtual boole setup_vertices(const vector<string>& vertices_viewer_vec) override;

    virtual boole load_resource() override;

    virtual boole unload_resource() override;

    virtual boole render() override;

private:
    obs<vulkan_runtime>             _rt;
    obs<vulkan_window_context>      _window_ctx;
    VkDescriptorSetLayout           _dynamic_memory_layout;
    VkPipelineLayout                _layout;
    VkRenderPass                    _render_pass;
    VkPipeline                      _pipeline;
    vector<VkFramebuffer>           _frame_buffer_vec;
    vector<VkCommandBuffer>         _command_buffer_vec;
    vector<VkBuffer>                _vk_vertices_buffers;
    vector<VkBuffer>                _vk_indices_buffers;
    vector<vertices_buffer_header>  _vertices_buffer_headers;
};

}
