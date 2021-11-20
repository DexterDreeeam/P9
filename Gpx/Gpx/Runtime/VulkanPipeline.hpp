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

private:
    obs<vulkan_window_context>  _window_ctx;
    VkPipelineLayout            _layout;
    VkRenderPass                _render_pass;
    VkPipeline                  _pipeline;
};

}
