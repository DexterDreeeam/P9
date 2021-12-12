
#include "../VulkanInterface.hpp"

namespace gpx
{

ref<vulkan_render_flow> vulkan_render_flow::build(const vulkan_render_flow_desc& desc)
{
    ref<vulkan_render_flow> rp;

    switch (desc.type)
    {
    case pipeline_render_type::basic:
        rp = ref<vulkan_render_flow_basic>::new_instance();
        if (rp->setup(desc))
        {
            return rp;
        }
        break;

    default:
        break;
    }

    return ref<vulkan_render_flow>();
}

boole vulkan_render_flow_basic::setup(const vulkan_render_flow_desc& desc)
{
    escape_function ef_error_handling =
        [&]() mutable
        {
            vkDestroyRenderPass(desc.logical_device, _render_pass, nullptr);
            _render_pass = nullptr;

            while (_frame_buffer_vec.size())
            {
                vkDestroyFramebuffer(desc.logical_device, _frame_buffer_vec.back(), nullptr);
                _frame_buffer_vec.pop_back();
            }
        };

    boole msaa_enable = desc.msaa_count != VK_SAMPLE_COUNT_1_BIT;

    vector<VkAttachmentDescription> attachment_vec;

    VkAttachmentDescription outputAttachment = {};
    outputAttachment.format = desc.surface_format;
    outputAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    outputAttachment.loadOp = msaa_enable ? VK_ATTACHMENT_LOAD_OP_DONT_CARE : VK_ATTACHMENT_LOAD_OP_CLEAR;
    outputAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    outputAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    outputAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    outputAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    outputAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachment_vec.push_back(outputAttachment);

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = desc.depth_format;
    depthAttachment.samples = desc.msaa_count;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachment_vec.push_back(depthAttachment);

    if (msaa_enable)
    {
        VkAttachmentDescription msaaAttachment = {};
        msaaAttachment.format = desc.surface_format;
        msaaAttachment.samples = desc.msaa_count;
        msaaAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        msaaAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        msaaAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        msaaAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        msaaAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        msaaAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment_vec.push_back(msaaAttachment);
    }

    VkAttachmentReference outputAttachmentRef = {};
    outputAttachmentRef.attachment = 0;
    outputAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference msaaAttachmentRef = {};
    msaaAttachmentRef.attachment = 2;
    msaaAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // pipeline render pass setting
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = msaa_enable ? &msaaAttachmentRef : &outputAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = msaa_enable ? &outputAttachmentRef : nullptr;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachment_vec.size();
    renderPassInfo.pAttachments = attachment_vec.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(desc.logical_device, &renderPassInfo, nullptr, &_render_pass) != VK_SUCCESS)
    {
        return boole::False;
    }

    for (s64 i = 0; i < desc.images_desc.frame_count; ++i)
    {
        vector<VkImageView> attach_vec;
        attach_vec.push_back(desc.images_desc.output_images[i]);
        attach_vec.push_back(desc.images_desc.depth_stencil_images[i]);
        if (msaa_enable)
        {
            attach_vec.push_back(desc.images_desc.msaa_images[i]);
        }

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _render_pass;
        framebufferInfo.attachmentCount = attach_vec.size();
        framebufferInfo.pAttachments = attach_vec.data();
        framebufferInfo.width = desc.render_area.width;
        framebufferInfo.height = desc.render_area.height;
        framebufferInfo.layers = 1;

        VkFramebuffer fb;
        if (vkCreateFramebuffer(
                desc.logical_device, &framebufferInfo, nullptr, &fb) != VK_SUCCESS)
        {
            return boole::False;
        }
        _frame_buffer_vec.push_back(fb);
    }

    _logical_device = desc.logical_device;
    _render_area = desc.render_area;
    _msaa_enable = msaa_enable;
    ef_error_handling.disable();
    return boole::True;
}

VkRenderPassBeginInfo vulkan_render_flow_basic::get_vk_begin_info(s64 img_idx) const
{
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _render_pass;
    renderPassInfo.framebuffer = _frame_buffer_vec[img_idx];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = _render_area;
    renderPassInfo.clearValueCount = 3;
    renderPassInfo.pClearValues = _clear_values;

    return renderPassInfo;
}

}
