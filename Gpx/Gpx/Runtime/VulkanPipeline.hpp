#pragma once

#include "Interface.hpp"

namespace gpx
{

// generate one pipeline_frame_package
struct pipeline_frame_package
{
    pipeline_frame_package() :
        _descriptor_pool(nullptr),
        _descriptor_set(nullptr),
        _command_buffer(nullptr)
    {
    }

    VkDescriptorPool       _descriptor_pool;
    VkDescriptorSet        _descriptor_set;
    VkCommandBuffer        _command_buffer;
};

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

    virtual boole setup_dynamic_memory(const vector<string>& dm_vec) override;

    virtual boole update_texture_viewer(const vector<string>& tv_vec) override;

    virtual boole setup_vertices(const vector<string>& vertices_viewer_vec) override;

    virtual boole load_resource() override;

    virtual boole unload_resource() override;

    virtual boole render() override;

public:
    void alert_dynamic_memory_change(obs<vulkan_dynamic_memory> ob_dm);

private:
    void update_dynamic_memory(s64 image_idx);

    void update_texture_viewer_internal(s64 image_idx, const vector<string>& tv_vec);

    void update_texture_viewer(s64 image_idx);

private:
    pipeline_desc                   _desc;
    obs<vulkan_pipeline>            _self;

    obs<vulkan_runtime>             _rt;
    obs<vulkan_window_context>      _window_ctx;

    // stages : init -> setup -> load_resource -> running_update

    VkDescriptorSetLayout           _descriptor_set_layout;           // init
    VkPipelineLayout                _layout;                          // init
    VkRenderPass                    _render_pass;                     // init
    VkPipeline                      _pipeline;                        // init
    vector<VkFramebuffer>           _frame_buffer_vec;                // init

    vector<
        ref<vulkan_dynamic_memory>
    >                               _dynamic_memory_vec;              // setup
    vector<VkBuffer>                _dynamic_memory_buffer_vec;       // setup
    vector<VkDeviceMemory>          _dynamic_memory_memory_vec;       // setup

    vector<VkBuffer>                _vk_vertices_buffers;             // setup
    vector<VkBuffer>                _vk_indices_buffers;              // setup
    vector<vertices_buffer_header>  _vertices_buffer_headers;         // setup

    vector<
        pipeline_frame_package
    >                               _frame_package_vec;               // init, load, update

    vector<
        pair<mutex, set<ref<vulkan_dynamic_memory>>>
    >                               _dynamic_memory_updating_queue;   // update

    vector<
        pair<mutex, map<s64, ref<vulkan_texture_viewer>>>
    >                               _texture_viewer_updating_queue;   // setup, update
};

}
