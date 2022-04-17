#pragma once

namespace gpx
{

struct vulkan_render_flow_image_desc
{
    s64                            frame_count;
    vector<VkImageView>            output_images;
    vector<VkImageView>            depth_stencil_images;
    vector<VkImageView>            msaa_images; // necessary when msaa_count is not VK_SAMPLE_COUNT_1_BIT
};

struct vulkan_render_flow_desc
{
    pipeline_render_type           type;
    VkDevice                       logical_device;
    VkExtent2D                     render_area;
    VkFormat                       surface_format;
    VkFormat                       depth_format;
    VkSampleCountFlagBits          msaa_count;
    vulkan_render_flow_image_desc  images_desc;
};

class vulkan_render_flow
{
public:
    static ref<vulkan_render_flow> build(const vulkan_render_flow_desc& desc);

public:
    vulkan_render_flow() :
        _logical_device(nullptr),
        _render_area(),
        _msaa_enable(boole::False),
        _render_pass(nullptr),
        _frame_buffer_vec()
    {
    }

    vulkan_render_flow(const vulkan_render_flow&) = delete;

    virtual ~vulkan_render_flow()
    {
        if (_render_pass)
        {
            vkDestroyRenderPass(_logical_device, _render_pass, nullptr);
        }
        while (_frame_buffer_vec.size())
        {
            vkDestroyFramebuffer(_logical_device, _frame_buffer_vec.back(), nullptr);
            _frame_buffer_vec.pop_back();
        }
    }

public:
    VkRenderPass get_vk_render_pass() { return _render_pass; }

    virtual boole setup(const vulkan_render_flow_desc& desc) = 0;

    virtual VkRenderPassBeginInfo get_vk_begin_info(s64 img_idx) const = 0;

protected:
    VkDevice                 _logical_device;
    VkExtent2D               _render_area;
    boole                    _msaa_enable;
    VkRenderPass             _render_pass;
    vector<VkFramebuffer>    _frame_buffer_vec;
};

class vulkan_render_flow_basic : public vulkan_render_flow
{
public:
    vulkan_render_flow_basic() :
        vulkan_render_flow()
    {
        // output, depth, msaa
        _clear_values[0].color        = { 0.008f, 0.008f, 0.012f, 1.0f };
        _clear_values[1].depthStencil = { 1.0f, 0 };
        _clear_values[2].color        = { 0.008f, 0.008f, 0.012f, 1.0f };
    }

    virtual ~vulkan_render_flow_basic() override = default;

    virtual boole setup(const vulkan_render_flow_desc& desc) override;

    virtual VkRenderPassBeginInfo get_vk_begin_info(s64 img_idx) const override;

private:
    VkClearValue    _clear_values[3];
};

}
