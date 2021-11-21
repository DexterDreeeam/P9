#pragma once

#include "Interface.hpp"

namespace gpx
{

struct vulkan_window_context;

class vulkan_pipeline : public pipeline
{
public:
    vulkan_pipeline(obs<vulkan_window_context> w_ctx);

    virtual ~vulkan_pipeline() override;

    virtual boole init(const pipeline_desc& desc) override;

    virtual boole uninit() override;

    virtual boole load_resource() override;

    virtual boole unload_resource() override;

    virtual boole render() override;

private:
    obs<vulkan_window_context>  _window_ctx;
    VkPipelineLayout            _layout;
    VkRenderPass                _render_pass;
    VkPipeline                  _pipeline;
    vector<VkFramebuffer>       _frame_buffer_vec;
    vector<VkCommandBuffer>     _command_buffer_vec;
    VkSemaphore                 _sema_image_available;
    VkSemaphore                 _sema_render_complete;
};

}
