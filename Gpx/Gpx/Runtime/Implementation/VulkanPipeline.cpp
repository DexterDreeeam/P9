
#include "../VulkanPipeline.hpp"
#include "../VulkanShader.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_pipeline::vulkan_pipeline(obs<vulkan_window_context> w_ctx) :
    _window_ctx(w_ctx),
    _layout(nullptr),
    _render_pass(nullptr),
    _pipeline(nullptr),
    _frame_buffer_vec(),
    _command_buffer_vec(),
    _sema_image_available(nullptr),
    _sema_render_complete(nullptr)
{
}

vulkan_pipeline::~vulkan_pipeline()
{
    uninit();
}

// there are 4 major steps we should do to build graphics pipeline
// 1. build programmable stage, such as vertex shader, fragment shader
// 2. build fixed-function stage config, such as input assembly format, rasterizer config
// 3. specify pipeline layout, indicate our vertices data or uniform variable
// 4. specify frame buffer and render pass, indicate buffers used in renderring process and define render pass flow
boole vulkan_pipeline::init(const pipeline_desc& desc)
{
    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    // pipeline shader stage
    vector<VkPipelineShaderStageCreateInfo> stage_create_info_vec;
    vector<ref<shader>> shaders = desc._shaders;
    shaders.sort(
        [](ref<shader> s1, ref<shader> s2)
        {
            return s1->type() < s2->type();
        }
    );
    for (auto r_shader : shaders)
    {
        VkShaderStageFlagBits vk_stage;
        auto vk_shader = r_shader.ref_of<vulkan_shader>();
        switch (vk_shader->_desc._type)
        {
        case shader_type::VERTEX:
        vk_stage = VK_SHADER_STAGE_VERTEX_BIT;
        break;
        case shader_type::TESSELLATION:
        vk_stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        assert(0);
        break;
        case shader_type::GEOMETRY:
        vk_stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        assert(0);
        break;
        case shader_type::FRAGMENT:
        vk_stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
        default:
        assert(0);
        break;
        }
        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = vk_stage;
        vertShaderStageInfo.module = vk_shader->_shader_module;
        vertShaderStageInfo.pName = "main";
        stage_create_info_vec.push_back(vertShaderStageInfo);
    }

    // pipeline vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // pipeline viewport
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (f32)w_ctx->_swap_chain_extent.width;
    viewport.height = (f32)w_ctx->_swap_chain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = w_ctx->_swap_chain_extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // pipeline rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // pipeline multi-sampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // pipeline depth/stencil buffer setting
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS; // near[0.0] to far[1.0]

    // pipeline color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // pipeline dynamic variable setting
    /*
    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    */

    // pipeline layout setting, including uniform variable
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(w_ctx->_logical_device, &pipelineLayoutInfo, nullptr, &_layout) != VK_SUCCESS)
    {
        uninit();
        return boole::False;
    }

    // pipeline frame buffer setting
    vector<VkAttachmentDescription> colorAttachment_vec;
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = w_ctx->_surface_format.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment_vec.push_back(colorAttachment);

    // pipeline render pass setting
    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = colorAttachment_vec.size();
    renderPassInfo.pAttachments = colorAttachment_vec.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(w_ctx->_logical_device, &renderPassInfo, nullptr, &_render_pass) != VK_SUCCESS)
    {
        uninit();
        return boole::False;
    }

    // pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stage_create_info_vec.size();
    pipelineInfo.pStages = stage_create_info_vec.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = _layout;
    pipelineInfo.renderPass = _render_pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(
            w_ctx->_logical_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
    {
        uninit();
        return boole::False;
    }

    // semaphore
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(
            w_ctx->_logical_device, &semaphoreInfo, nullptr, &_sema_image_available) != VK_SUCCESS)
    {
        uninit();
        return boole::False;
    }
    if (vkCreateSemaphore(
            w_ctx->_logical_device, &semaphoreInfo, nullptr, &_sema_render_complete) != VK_SUCCESS)
    {
        uninit();
        return boole::False;
    }

    return boole::True;
}

boole vulkan_pipeline::uninit()
{
    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }
    if (_sema_image_available)
    {
        vkDestroySemaphore(w_ctx->_logical_device, _sema_image_available, nullptr);
        vkDestroySemaphore(w_ctx->_logical_device, _sema_render_complete, nullptr);
        _sema_image_available = nullptr;
        _sema_render_complete = nullptr;
    }
    if (_pipeline)
    {
        vkDestroyPipeline(w_ctx->_logical_device, _pipeline, nullptr);
        _pipeline = nullptr;
    }
    if (_render_pass)
    {
        vkDestroyRenderPass(w_ctx->_logical_device, _render_pass, nullptr);
        _render_pass = nullptr;
    }
    if (_layout)
    {
        vkDestroyPipelineLayout(w_ctx->_logical_device, _layout, nullptr);
        _layout = nullptr;
    }
    return boole::True;
}

boole vulkan_pipeline::load_resource()
{
    if (!_pipeline || !_render_pass || !_layout)
    {
        return boole::False;
    }

    if (_frame_buffer_vec.size() > 0 || _command_buffer_vec.size() > 0)
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    s64 image_count = w_ctx->_image_view_vec.size();
    VkCommandBufferAllocateInfo allocInfo = {};

    // frame buffer
    for (s64 i = 0; i < image_count; ++i)
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &w_ctx->_image_view_vec[i];
        framebufferInfo.width = w_ctx->_swap_chain_extent.width;
        framebufferInfo.height = w_ctx->_swap_chain_extent.height;
        framebufferInfo.layers = 1;

        VkFramebuffer fb;
        if (vkCreateFramebuffer(
                w_ctx->_logical_device, &framebufferInfo, nullptr, &fb) != VK_SUCCESS)
        {
            goto L_error;
        }
        _frame_buffer_vec.push_back(fb);
    }

    // allocate command buffer
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = w_ctx->_command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)image_count;
    _command_buffer_vec.resize(image_count, nullptr);
    if (vkAllocateCommandBuffers(
            w_ctx->_logical_device, &allocInfo, _command_buffer_vec.data()) != VK_SUCCESS)
    {
        _command_buffer_vec.clear();
        goto L_error;
    }

    // record command buffer
    for (s64 i = 0; i < image_count; ++i)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(_command_buffer_vec[i], &beginInfo) != VK_SUCCESS)
        {
            goto L_error;
        }

        VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _render_pass;
        renderPassInfo.framebuffer = _frame_buffer_vec[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = w_ctx->_swap_chain_extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(_command_buffer_vec[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(_command_buffer_vec[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
        vkCmdDraw(_command_buffer_vec[i], 3, 1, 0, 0);
        vkCmdEndRenderPass(_command_buffer_vec[i]);

        if (vkEndCommandBuffer(_command_buffer_vec[i]) != VK_SUCCESS)
        {
            goto L_error;
        }
    }

    return boole::True;

L_error:
    unload_resource();
    return boole::False;
}

boole vulkan_pipeline::unload_resource()
{
    if (!_pipeline || !_render_pass || !_layout)
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    if (_command_buffer_vec.size())
    {
        vkFreeCommandBuffers(
            w_ctx->_logical_device,
            w_ctx->_command_pool,
            _command_buffer_vec.size(),
            _command_buffer_vec.data());
        _command_buffer_vec.clear();
    }
    while (_frame_buffer_vec.size())
    {
        vkDestroyFramebuffer(w_ctx->_logical_device, _frame_buffer_vec.back(), nullptr);
        _frame_buffer_vec.pop_back();
    }
    return boole::True;
}

//boole vulkan_pipeline::render()
//{
//    auto w_ctx = _window_ctx.try_ref();
//    if (w_ctx.empty())
//    {
//        return boole::False;
//    }
//
//    u64 image_index = 0;
//    if (vkAcquireNextImageKHR(
//            w_ctx->_logical_device,
//            w_ctx->_swap_chain,
//            u64_max,
//            _sema_image_available,
//            nullptr,
//            (uint32_t*)&image_index) != VK_SUCCESS)
//    {
//        return boole::False;
//    }
//
//    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
//    VkSubmitInfo submitInfo = {};
//    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//    submitInfo.waitSemaphoreCount = 1;
//    submitInfo.pWaitSemaphores = &_sema_image_available;
//    submitInfo.pWaitDstStageMask = waitStages;
//    submitInfo.commandBufferCount = 1;
//    submitInfo.pCommandBuffers = &_command_buffer_vec[image_index];
//    submitInfo.signalSemaphoreCount = 1;
//    submitInfo.pSignalSemaphores = &_sema_render_complete;
//
//    if (vkQueueSubmit(w_ctx->_render_queue, 1, &submitInfo, nullptr) != VK_SUCCESS)
//    {
//        return boole::False;
//    }
//
//    VkPresentInfoKHR presentInfo{};
//    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//    presentInfo.waitSemaphoreCount = 1;
//    presentInfo.pWaitSemaphores = &_sema_render_complete;
//    presentInfo.swapchainCount = 1;
//    presentInfo.pSwapchains = &w_ctx->_swap_chain;
//    presentInfo.pImageIndices = (uint32_t*)&image_index;
//    presentInfo.pResults = nullptr;
//
//    if (vkQueuePresentKHR(w_ctx->_render_queue, &presentInfo) != VK_SUCCESS)
//    {
//        return boole::False;
//    }
//
//    return boole::True;
//}

vector<VkSemaphore> imageAvailableSemaphores;
vector<VkSemaphore> renderFinishedSemaphores;
vector<VkFence> inFlightFences;
vector<VkFence> imagesInFlight;
const int MAX_FRAMES_IN_FLIGHT = 2;
size_t currentFrame = 0;

boole vulkan_pipeline::render()
{
    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT, nullptr);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT, nullptr);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT, nullptr);
    imagesInFlight.resize(3, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(w_ctx->_logical_device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(w_ctx->_logical_device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(w_ctx->_logical_device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw 123;
        }
    }

    while (1)
    {

        vkWaitForFences(w_ctx->_logical_device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(w_ctx->_logical_device, w_ctx->_swap_chain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(w_ctx->_logical_device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_command_buffer_vec[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(w_ctx->_logical_device, 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(w_ctx->_render_queue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw 234;
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {w_ctx->_swap_chain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(w_ctx->_present_queue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        w_ctx->_window->poll_event();
    }

    return boole::True;
}

}
