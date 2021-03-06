
#include "../VulkanInterface.hpp"

namespace gpx
{

vulkan_pipeline::vulkan_pipeline(obs<vulkan_runtime> rt, obs<vulkan_window_context> w_ctx) :
    _desc(),
    _self(),
    _rt(rt),
    _window_ctx(w_ctx),
    _descriptor_set_layout(nullptr),
    _layout(nullptr),
    _r_render_flow(),
    _pipeline(nullptr),
    _dynamic_memory_vec(),
    _dynamic_memory_buffer_vec(),
    _dynamic_memory_memory_vec(),
    _vk_vertices_buffers(),
    _vk_indices_buffers(),
    _vertices_buffer_headers(),
    _frame_package_vec(),
    _buffer_info_vec(),
    _image_info_vec(),
    _write_ds_vec(),
    _dynamic_memory_updating_queue(),
    _texture_viewer_updating_queue()
{
}

vulkan_pipeline::~vulkan_pipeline()
{
    AUTO_TRACE;

    uninit();
}

// there are 4 major steps we should do to build graphics pipeline
// 1. build programmable stage, such as vertex shader, fragment shader
// 2. build fixed-function stage config, such as input assembly format, rasterizer config
// 3. specify pipeline layout, indicate our vertices data or uniform variable
// 4. specify frame buffer and render pass, indicate buffers used in renderring process and define render pass flow
boole vulkan_pipeline::init(const pipeline_desc& desc, obs<pipeline> self)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    _self = self.obs_of<vulkan_pipeline>();

    if (!transfer_state(pipeline_state::Uninit, pipeline_state::InitingOrUniniting))
    {
        return boole::False;
    }
    auto logical_device = rt->get_vk_logical_device();
    s64 image_count = rt->_desc.frame_count;
    boole msaa_enable = w_ctx->_msaa_image_vec.size() == image_count;

    escape_function ef_uninit =
        [=]() mutable
        {
            uninit();
        };

    // pipeline shader stage
    vector<VkPipelineShaderStageCreateInfo> stage_create_info_vec;
    vector<ref<shader>> shaders = desc.shaders;
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
        switch (vk_shader->_desc.type)
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
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = vulkan_vertex_supporter::get_vk_info(desc.vertex_type);

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
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // pipeline multi-sampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = rt->_msaa;
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
    depthStencil.depthBoundsTestEnable = VK_FALSE; // custimized depth bound, instead of 0 -> 1
    depthStencil.minDepthBounds = 0;
    depthStencil.maxDepthBounds = 1;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};

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

    // descriptor
    _frame_package_vec.resize(image_count);

    s64 dynamic_memory_cnt = desc.dynamic_memory_count;
    s64 texture_viewer_cnt = desc.texture_viewer_count;
    if (dynamic_memory_cnt + texture_viewer_cnt > 0)
    {
        s64 binding = 0;

        vector<VkDescriptorSetLayoutBinding> layout_binding_vec;
        VkDescriptorSetLayoutBinding layout_binding = {};
        layout_binding.descriptorCount = 1;
        layout_binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
        layout_binding.pImmutableSamplers = nullptr;

        for (s64 i = 0; i < dynamic_memory_cnt; ++i)
        {
            layout_binding.binding = binding++;
            layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            layout_binding_vec.push_back(layout_binding);
        }
        for (s64 i = 0; i < texture_viewer_cnt; ++i)
        {
            layout_binding.binding = binding++;
            layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            layout_binding_vec.push_back(layout_binding);
        }

        VkDescriptorSetLayoutCreateInfo ds_layout_info = {};
        ds_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        ds_layout_info.bindingCount = layout_binding_vec.size();
        ds_layout_info.pBindings = layout_binding_vec.data();

        if (vkCreateDescriptorSetLayout(
                logical_device, &ds_layout_info, nullptr, &_descriptor_set_layout) != VK_SUCCESS)
        {
            return boole::False;
        }

        vector<VkDescriptorPoolSize> pool_size_vec;
        if (dynamic_memory_cnt > 0)
        {
            VkDescriptorPoolSize pool_size = {};
            pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pool_size.descriptorCount = dynamic_memory_cnt;
            pool_size_vec.push_back(pool_size);
        }
        if (texture_viewer_cnt > 0)
        {
            VkDescriptorPoolSize pool_size = {};
            pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            pool_size.descriptorCount = texture_viewer_cnt;
            pool_size_vec.push_back(pool_size);
        }

        VkDescriptorPoolCreateInfo descriptor_pool_info = {};
        descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptor_pool_info.poolSizeCount = pool_size_vec.size();
        descriptor_pool_info.pPoolSizes = pool_size_vec.data();
        descriptor_pool_info.maxSets = 2; // allow every frame generate another ds set for bufferring

        for (s64 i = 0; i < image_count; ++i)
        {
            if (vkCreateDescriptorPool(
                    logical_device, &descriptor_pool_info, nullptr, &_frame_package_vec[i]._descriptor_pool) != VK_SUCCESS)
            {
                return boole::False;
            }

            mutex m_dm;
            m_dm.init();
            _dynamic_memory_updating_queue.push_back(make_pair(m_dm, set<ref<vulkan_dynamic_memory>>()));

            mutex m_tv;
            m_tv.init();
            _texture_viewer_updating_queue.push_back(make_pair(m_tv, vector<ref<vulkan_texture_viewer>>()));
        }
    }

    // pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = _descriptor_set_layout != nullptr ? 1 : 0;
    pipelineLayoutInfo.pSetLayouts = &_descriptor_set_layout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(logical_device, &pipelineLayoutInfo, nullptr, &_layout) != VK_SUCCESS)
    {
        return boole::False;
    }

    // render_flow [render pass + frame buffer]
    vulkan_render_flow_desc rf_desc;
    rf_desc.type = desc.render_type;
    rf_desc.logical_device = logical_device;
    rf_desc.render_area = w_ctx->_swap_chain_extent;
    rf_desc.surface_format = w_ctx->_surface_format.format;
    rf_desc.depth_format = w_ctx->_depth_format;
    rf_desc.msaa_count = rt->_msaa;
    rf_desc.images_desc.frame_count = image_count;
    rf_desc.images_desc.output_images = w_ctx->_image_view_vec;
    rf_desc.images_desc.depth_stencil_images = w_ctx->_depth_image_view_vec;
    rf_desc.images_desc.msaa_images = w_ctx->_msaa_image_view_vec;

    _r_render_flow = vulkan_render_flow::build(rf_desc);
    if (_r_render_flow.empty())
    {
        return boole::False;
    }

    // pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
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
    pipelineInfo.renderPass = _r_render_flow->get_vk_render_pass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(
            logical_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
    {
        return boole::False;
    }

    _desc = desc;
    ef_uninit.disable();
    transfer_state(pipeline_state::InitingOrUniniting, pipeline_state::Inited);
    return boole::True;
}

boole vulkan_pipeline::uninit()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    transfer_state(pipeline_state::Inited, pipeline_state::InitingOrUniniting);
    auto logical_device = rt->get_vk_logical_device();

    _vk_vertices_buffers.clear();
    _vk_indices_buffers.clear();
    _vertices_buffer_headers.clear();

    if (_pipeline)
    {
        vkDestroyPipeline(logical_device, _pipeline, nullptr);
        _pipeline = nullptr;
    }
    if (_r_render_flow.has_value())
    {
        _r_render_flow.clear();
    }
    if (_layout)
    {
        vkDestroyPipelineLayout(logical_device, _layout, nullptr);
        _layout = nullptr;
    }
    while (_texture_viewer_updating_queue.size())
    {
        _texture_viewer_updating_queue.back().first.uninit();
        _texture_viewer_updating_queue.pop_back();
    }
    while (_dynamic_memory_updating_queue.size())
    {
        _dynamic_memory_updating_queue.back().first.uninit();
        _dynamic_memory_updating_queue.pop_back();
    }

    for (auto& pkg : _frame_package_vec)
    {
        vkDestroyDescriptorPool(logical_device, pkg._descriptor_pool, nullptr);
        pkg._descriptor_pool = nullptr;
    }
    if (_descriptor_set_layout)
    {
        vkDestroyDescriptorSetLayout(logical_device, _descriptor_set_layout, nullptr);
        _descriptor_set_layout = nullptr;
    }

    boole checker = transfer_state(pipeline_state::InitingOrUniniting, pipeline_state::Uninit);
    assert(checker);
    return boole::True;
}

boole vulkan_pipeline::setup_dynamic_memory(const vector<string>& dm_vec)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();

    escape_function ef_error =
        [&]() mutable
        {
            for (auto r_dm : _dynamic_memory_vec)
            {
                r_dm->remove_pipeline_memory(_self);
            }
            while (_dynamic_memory_buffer_vec.size())
            {
                vulkan_runtime::clear_vk_buffer(
                    logical_device, _dynamic_memory_buffer_vec.back(), _dynamic_memory_memory_vec.back());

                _dynamic_memory_buffer_vec.pop_back();
                _dynamic_memory_memory_vec.pop_back();
            }
            _dynamic_memory_vec.clear();
        };

    for (auto& dm : dm_vec)
    {
        auto r_dm = rt->get_dynamic_memory(dm);
        if (r_dm.empty())
        {
            _dynamic_memory_vec.clear();
            return boole::False;
        }
        _dynamic_memory_vec.push_back(r_dm);
    }

    s64 image_count = rt->_desc.frame_count;
    for (s64 i = 0; i < image_count; ++i)
    {
        for (auto r_dm : _dynamic_memory_vec)
        {
            VkBuffer buffer;
            VkDeviceMemory memory;
            if (!vulkan_runtime::setup_vk_buffer(
                rt->get_vk_logical_device(),
                rt->get_vk_physical_device(),
                r_dm->memory_size(),
                rt->_render_queue_family_idx == rt->_transfer_queue_family_idx,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                buffer,
                memory))
            {
                return boole::False;
            }
            _dynamic_memory_buffer_vec.push_back(buffer);
            _dynamic_memory_memory_vec.push_back(memory);

            if (!r_dm->add_pipeline_memory(_self, memory))
            {
                return boole::False;
            }
        }
    }

    ef_error.disable();
    return boole::True;
}

boole vulkan_pipeline::update_texture_viewer(const vector<string>& tv_vec)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    sz_t image_count = rt->_desc.frame_count;
    s64 updated_count = 0;
    vector<s64> update_indices(image_count, 0);

    while (updated_count != image_count)
    {
        if (updated_count == image_count - 1)
        {
            s64 img = 0;
            while (update_indices[img] == 1)
            {
                ++img;
            }
            _texture_viewer_updating_queue[img].first.wait_acquire();
            bind_texture_viewer(img, tv_vec);
            _texture_viewer_updating_queue[img].first.release();
            update_indices[img] = 1;
            ++updated_count;
        }
        else
        {
            s64 img = 0;
            while (img < image_count && update_indices[img] == 0)
            {
                if (_texture_viewer_updating_queue[img].first.try_acquire())
                {
                    bind_texture_viewer(img, tv_vec);
                    _texture_viewer_updating_queue[img].first.release();
                    update_indices[img] = 1;
                    ++updated_count;
                }
                ++img;
            }
        }
    }
    return boole::True;
}

boole vulkan_pipeline::setup_vertices(const vector<string>& vertices_viewer_vec)
{
    AUTO_TRACE;
    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    if (!transfer_state(pipeline_state::Inited, pipeline_state::SetupInProgress))
    {
        return boole::False;
    }
    escape_function ef_state =
        [=]() mutable
        {
            transfer_state(pipeline_state::SetupInProgress, pipeline_state::Inited);
        };

    _vk_vertices_buffers.clear();
    _vk_indices_buffers.clear();
    _vertices_buffer_headers.clear();
    for (auto& vv : vertices_viewer_vec)
    {
        auto r_vv = rt->get_vertices_viewer(vv);
        if (r_vv->state() != vertices_viewer_state::Online)
        {
            _vk_vertices_buffers.clear();
            _vk_indices_buffers.clear();
            _vertices_buffer_headers.clear();
            return boole::False;
        }
        _vk_vertices_buffers.push_back(r_vv->_vk_buffer);
        _vk_indices_buffers.push_back(r_vv->_vk_buffer_indices);
        _vertices_buffer_headers.push_back(r_vv->_vb_header);
    }

    return boole::True;
}

boole vulkan_pipeline::load_resource()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    s64 image_count = rt->_desc.frame_count;
    auto logical_device = rt->get_vk_logical_device();

    escape_function ef_error_handle =
        [&]() mutable
        {
            for (s64 img = 0; img < image_count; ++img)
            {
                if (_frame_package_vec[img]._descriptor_set)
                {
                    vkFreeDescriptorSets(
                        logical_device, _frame_package_vec[img]._descriptor_pool, 1, &_frame_package_vec[img]._descriptor_set);

                    _frame_package_vec[img]._descriptor_set = nullptr;
                }

                if (_frame_package_vec[img]._command_buffer)
                {
                    vkFreeCommandBuffers(logical_device, rt->_render_command_pool, 1, &_frame_package_vec[img]._command_buffer);
                    _frame_package_vec[img]._command_buffer = nullptr;
                }
            }
        };

    s64 dm_count = _desc.dynamic_memory_count;
    s64 tv_count = _desc.texture_viewer_count;
    _buffer_info_vec.resize(image_count * dm_count);
    _image_info_vec.resize(image_count * tv_count);

    if (dm_count + tv_count > 0)
    {
        for (s64 img = 0; img < image_count; ++img)
        {
            VkDescriptorSetAllocateInfo descriptor_set_alloc_info = {};
            descriptor_set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptor_set_alloc_info.descriptorPool = _frame_package_vec[img]._descriptor_pool;
            descriptor_set_alloc_info.descriptorSetCount = 1;
            descriptor_set_alloc_info.pSetLayouts = &_descriptor_set_layout;

            if (vkAllocateDescriptorSets(
                logical_device, &descriptor_set_alloc_info, &_frame_package_vec[img]._descriptor_set) != VK_SUCCESS)
            {
                return boole::False;
            }

            _write_ds_vec.push_back(vector<VkWriteDescriptorSet>());
            s64 binding = 0;
            for (s64 i = 0; i < _desc.dynamic_memory_count; ++i)
            {
                auto& bufferInfo = _buffer_info_vec[img * _desc.dynamic_memory_count + i];
                bufferInfo.buffer = _dynamic_memory_buffer_vec[img * _desc.dynamic_memory_count + i];
                bufferInfo.offset = 0;
                bufferInfo.range = _dynamic_memory_vec[i]->memory_size();

                VkWriteDescriptorSet write_ds = {};
                write_ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_ds.dstSet = _frame_package_vec[img]._descriptor_set;
                write_ds.dstBinding = binding++;
                write_ds.dstArrayElement = 0;
                write_ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write_ds.descriptorCount = 1;
                write_ds.pBufferInfo = &bufferInfo;
                _write_ds_vec.back().push_back(write_ds);
            }

            auto& tv_vec = _texture_viewer_updating_queue[img].second;
            if (tv_vec.size() != _desc.texture_viewer_count)
            {
                return boole::False;
            }
            for (s64 i = 0; i < _desc.texture_viewer_count; ++i)
            {
                auto tv = tv_vec[i];
                auto& imageInfo = _image_info_vec[img * _desc.texture_viewer_count + i];
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = tv->_vk_image_view;
                imageInfo.sampler = tv->_vk_sampler;

                VkWriteDescriptorSet write_ds = {};
                write_ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write_ds.dstSet = _frame_package_vec[img]._descriptor_set;
                write_ds.dstBinding = binding++;
                write_ds.dstArrayElement = 0;
                write_ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                write_ds.descriptorCount = 1;
                write_ds.pImageInfo = &imageInfo;
                _write_ds_vec.back().push_back(write_ds);
            }
        }
    }

    for (s64 img = 0; img < image_count; ++img)
    {
        if (!update_command_buffer(img))
        {
            return boole::False;
        }
    }

    ef_error_handle.disable();
    return boole::True;
}

boole vulkan_pipeline::unload_resource()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    s64 image_count = rt->_desc.frame_count;
    auto logical_device = rt->get_vk_logical_device();

    for (s64 img = 0; img < image_count; ++img)
    {
        if (_frame_package_vec[img]._descriptor_set)
        {
            if (vkFreeDescriptorSets(
                    logical_device, _frame_package_vec[img]._descriptor_pool, 1, &_frame_package_vec[img]._descriptor_set) != VK_SUCCESS)
            {
                return boole::False;
            }

            _frame_package_vec[img]._descriptor_set = nullptr;
        }

        if (_frame_package_vec[img]._command_buffer)
        {
            vkFreeCommandBuffers(logical_device, rt->_render_command_pool, 1, &_frame_package_vec[img]._command_buffer);
            _frame_package_vec[img]._command_buffer = nullptr;
        }
    }

    while (_dynamic_memory_vec.size())
    {
        if (!_dynamic_memory_vec.back()->remove_pipeline_memory(_self))
        {
            return boole::False;
        }
        _dynamic_memory_vec.pop_back();
    }

    while (_dynamic_memory_buffer_vec.size())
    {
        assert(_dynamic_memory_memory_vec.size());
        vulkan_runtime::clear_vk_buffer(logical_device, _dynamic_memory_buffer_vec.back(), _dynamic_memory_memory_vec.back());
        _dynamic_memory_buffer_vec.pop_back();
        _dynamic_memory_memory_vec.pop_back();
    }

    _vk_vertices_buffers.clear();
    _vk_indices_buffers.clear();
    _vertices_buffer_headers.clear();

    for (auto& q : _dynamic_memory_updating_queue)
    {
        q.second.clear();
    }
    for (auto& q : _texture_viewer_updating_queue)
    {
        q.second.clear();
    }
    _buffer_info_vec.clear();
    _image_info_vec.clear();
    _write_ds_vec.clear();

    return boole::True;
}

boole vulkan_pipeline::render()
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }

    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();
    s64 counter = w_ctx->_fence_counter;
    u64 image_index = 0;

    log("%s Wait In-Flight Fence [%ull] start", __FUNCTION__, counter);
    vkWaitForFences(logical_device, 1, &w_ctx->_inflight_fence_vec[counter], VK_TRUE, u64_max);
    log("%s Wait In-Flight Fence [%ull] end", __FUNCTION__, counter);

    log("%s Wait Acuire Next Image start", __FUNCTION__);
    if (vkAcquireNextImageKHR(
            logical_device,
            w_ctx->_swap_chain,
            u64_max,
            w_ctx->_image_available_sema_vec[counter],
            nullptr,
            (uint32_t*)&image_index) != VK_SUCCESS)
    {
        return boole::False;
    }
    log("%s Wait Acuire Next Image [%ull] end", __FUNCTION__, image_index);

    if (w_ctx->_reordered_fence_vec[image_index] && w_ctx->_reordered_fence_vec[image_index] != w_ctx->_inflight_fence_vec[counter])
    {
        log("%s Wait Re-Order Fence [%ull] start", __FUNCTION__, image_index);
        vkWaitForFences(logical_device, 1, &w_ctx->_reordered_fence_vec[image_index], VK_TRUE, u64_max);
        log("%s Wait Re-Order Fence [%ull] end", __FUNCTION__, image_index);
    }
    w_ctx->_reordered_fence_vec[image_index] = w_ctx->_inflight_fence_vec[counter];
    vkResetFences(logical_device, 1, &w_ctx->_inflight_fence_vec[counter]);

    update_dynamic_memory(image_index);
    update_texture_viewer(image_index);

    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &w_ctx->_image_available_sema_vec[counter];
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_frame_package_vec[image_index]._command_buffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &w_ctx->_render_complete_sema_vec[counter];

    if (vkQueueSubmit(rt->_render_queue, 1, &submitInfo, w_ctx->_inflight_fence_vec[counter]) != VK_SUCCESS)
    {
        return boole::False;
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &w_ctx->_render_complete_sema_vec[counter];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &w_ctx->_swap_chain;
    presentInfo.pImageIndices = (uint32_t*)&image_index;
    presentInfo.pResults = nullptr;

    if (vkQueuePresentKHR(rt->_present_queue, &presentInfo) != VK_SUCCESS)
    {
        return boole::False;
    }

    counter = (counter + 1) % w_ctx->_swap_chain_image_vec.size();
    w_ctx->_fence_counter = counter;
    return boole::True;
}

void vulkan_pipeline::alert_dynamic_memory_change(obs<vulkan_dynamic_memory> ob_dm)
{
    AUTO_TRACE;

    for (auto& p : _dynamic_memory_updating_queue)
    {
        auto r_dm = ob_dm.try_ref();
        if (r_dm.has_value())
        {
            p.first.wait_acquire();
            p.second.insert(r_dm);
            p.first.release();
        }
    }
}

void vulkan_pipeline::update_dynamic_memory(s64 image_idx)
{
    AUTO_TRACE;

    if (image_idx >= _dynamic_memory_updating_queue.size())
    {
        return;
    }

    log("%s update dynamic memory for frame [%ull]", __FUNCTION__, image_idx);
    auto& p = _dynamic_memory_updating_queue[image_idx];
    p.first.wait_acquire();
    escape_function ef_lock_release =
        [=]() mutable
        {
            p.first.release();
        };

    for (auto r_dm : p.second)
    {
        if (!r_dm->update_pipeline_memory(_self, image_idx))
        {
            assert(0);
        }
    }

    p.second.clear();
}

void vulkan_pipeline::bind_texture_viewer(s64 image_idx, const vector<string>& tv_vec)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        assert(0);
        return;
    }

    s64 dm_count = _desc.dynamic_memory_count;
    auto& update_queue = _texture_viewer_updating_queue[image_idx].second;
    update_queue.clear();

    for (s64 i = 0; i < tv_vec.size(); ++i)
    {
        auto tv = rt->get_texture_viewer(tv_vec[i]);
        if (tv.empty())
        {
            assert(0);
            return;
        }
        update_queue.push_back(tv.ref_of<vulkan_texture_viewer>());
    }
}

void vulkan_pipeline::update_texture_viewer(s64 image_idx)
{
    AUTO_TRACE;

    if (image_idx >= _texture_viewer_updating_queue.size())
    {
        return;
    }

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return;
    }

    auto& update_queue = _texture_viewer_updating_queue[image_idx].second;
    if (update_queue.size() == 0)
    {
        return;
    }

    auto logical_device = rt->get_vk_logical_device();
    auto lk = _texture_viewer_updating_queue[image_idx].first;
    lk.wait_acquire();
    escape_function ef_release_lock =
        [=]() mutable
        {
            lk.release();
        };

    s64 tv_count = _desc.texture_viewer_count;
    for (s64 i = 0; i < update_queue.size(); ++i)
    {
        auto tv = update_queue[i];
        _image_info_vec[image_idx * tv_count + i].imageView = tv->_vk_image_view;
        _image_info_vec[image_idx * tv_count + i].sampler = tv->_vk_sampler;
    }

    if (!update_command_buffer(image_idx))
    {
        assert(0);
    }
}

boole vulkan_pipeline::update_command_buffer(s64 image_idx)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }
    if (image_idx >= _frame_package_vec.size())
    {
        return boole::False;
    }
    auto w_ctx = _window_ctx.try_ref();
    if (w_ctx.empty())
    {
        return boole::False;
    }

    auto logical_device = rt->get_vk_logical_device();
    auto& pkg = _frame_package_vec[image_idx];

    if (pkg._command_buffer == nullptr)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = rt->_render_command_pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(
                logical_device, &allocInfo, &pkg._command_buffer) != VK_SUCCESS)
        {
            return boole::False;
        }
    }
    else
    {
        if (vkResetCommandBuffer(pkg._command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS)
        {
            return boole::False;
        }
    }

    s64 dm_count = _desc.dynamic_memory_count;
    s64 tv_count = _desc.texture_viewer_count;
    if (dm_count + tv_count > 0)
    {
        vkUpdateDescriptorSets(
            logical_device, _write_ds_vec[image_idx].size(), _write_ds_vec[image_idx].data(), 0, nullptr);
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;
    if (vkBeginCommandBuffer(pkg._command_buffer, &beginInfo) != VK_SUCCESS)
    {
        return boole::False;
    }

    // output, depth, msaa
    auto rp_begin_info = _r_render_flow->get_vk_begin_info(image_idx);

    vkCmdBeginRenderPass(pkg._command_buffer, &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(pkg._command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

    if (dm_count + tv_count > 0)
    {
        vkCmdBindDescriptorSets(
            pkg._command_buffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            _layout,
            0,
            1,
            &pkg._descriptor_set,
            0,
            nullptr);
    }

    vector<VkDeviceSize> offsets;
    for (s64 j = 0; j < _vertices_buffer_headers.size(); ++j)
    {
        s64 offset_idx = offsets.size();
        offsets.push_back(0);
        vkCmdBindVertexBuffers(pkg._command_buffer, 0, 1, &_vk_vertices_buffers[j], &offsets[offset_idx]);

        if (_vertices_buffer_headers[j].indices_count > 0)
        {
            vkCmdBindIndexBuffer(pkg._command_buffer, _vk_indices_buffers[j], 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(pkg._command_buffer, _vertices_buffer_headers[j].indices_count, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(pkg._command_buffer, _vertices_buffer_headers[j].vertices_count, 1, 0, 0);
        }
    }
    vkCmdEndRenderPass(pkg._command_buffer);
    if (vkEndCommandBuffer(pkg._command_buffer) != VK_SUCCESS)
    {
        return boole::False;
    }

    return boole::True;
}

}
