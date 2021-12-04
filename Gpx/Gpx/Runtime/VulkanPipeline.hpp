#pragma once

#define GLFW_INCLUDE_VULKAN
#include "../../External/Vulkan/Windows/Header/glfw3.h"
#include "Interface.hpp"

namespace gpx
{

class vulkan_runtime;

class vulkan_dynamic_memory;

struct vulkan_window_context;

class vulkan_pipeline : public pipeline
{
public:
    vulkan_pipeline(obs<vulkan_runtime> rt, obs<vulkan_window_context> w_ctx);

    virtual ~vulkan_pipeline() override;

    virtual boole init(const pipeline_desc& desc, obs<pipeline> self) override;

    virtual boole uninit() override;

    virtual boole setup_vertices(const vector<string>& vertices_viewer_vec) override;

    virtual boole load_resource() override;

    virtual boole unload_resource() override;

    virtual boole render() override;

public:
    void alert_dynamic_memory_change(obs<vulkan_dynamic_memory> ob_dm);

private:
    void update_dynamic_memory(s64 image_idx);

private:
    string                          _name;
    obs<vulkan_pipeline>            _self;

    obs<vulkan_runtime>             _rt;
    obs<vulkan_window_context>      _window_ctx;

    vector<VkDescriptorPool>        _descriptor_pool_vec;
    vector<VkDescriptorSet>         _descriptor_set_vec;
    vector<
        ref<vulkan_dynamic_memory>
    >                               _dynamic_memory_vec;
    vector<VkBuffer>                _dynamic_memory_buffer_vec;
    vector<VkDeviceMemory>          _dynamic_memory_memory_vec;

    vector<
        pair<mutex, set<ref<vulkan_dynamic_memory>>>
    >                               _dynamic_memory_updating_queue;


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
