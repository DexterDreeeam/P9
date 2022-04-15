#pragma once

namespace gpx
{

namespace _VkVertexSupportNs
{

template<vertex_type VertTy>
class vulkan_vertex_supporter_internal;

template<>
class vulkan_vertex_supporter_internal<vertex_type::POS_COLOR>
{
public:

    static array<VkVertexInputBindingDescription, 1> bindings;
    static array<VkVertexInputAttributeDescription, 2> attributes;

    static VkPipelineVertexInputStateCreateInfo get_vk_vertex_input_info()
    {
        VkPipelineVertexInputStateCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        info.vertexBindingDescriptionCount = 1;
        info.pVertexBindingDescriptions = bindings.data();
        info.vertexAttributeDescriptionCount = 2;
        info.pVertexAttributeDescriptions = attributes.data();
        return info;
    }
};

_SELECTANY_ array<VkVertexInputBindingDescription, 1> vulkan_vertex_supporter_internal<vertex_type::POS_COLOR>::bindings =
{
    { 0, sizeof(vertex_pos_color), VK_VERTEX_INPUT_RATE_VERTEX }
};

_SELECTANY_ array<VkVertexInputAttributeDescription, 2> vulkan_vertex_supporter_internal<vertex_type::POS_COLOR>::attributes =
{
    VkVertexInputAttributeDescription { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, (u32)class_offset(vertex_pos_color, _pos) },
    VkVertexInputAttributeDescription { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, (u32)class_offset(vertex_pos_color, _color) }
};

template<>
class vulkan_vertex_supporter_internal<vertex_type::POS_TEXTURE>
{
public:

    static array<VkVertexInputBindingDescription, 1>    bindings;
    static array<VkVertexInputAttributeDescription, 2>  attributes;

    static VkPipelineVertexInputStateCreateInfo get_vk_vertex_input_info()
    {
        static VkPipelineVertexInputStateCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        info.vertexBindingDescriptionCount = 1;
        info.pVertexBindingDescriptions = bindings.data();
        info.vertexAttributeDescriptionCount = 2;
        info.pVertexAttributeDescriptions = attributes.data();
        return info;
    }
};

_SELECTANY_ array<VkVertexInputBindingDescription, 1> vulkan_vertex_supporter_internal<vertex_type::POS_TEXTURE>::bindings =
{
    { 0, sizeof(vertex_pos_texture), VK_VERTEX_INPUT_RATE_VERTEX }
};

_SELECTANY_ array<VkVertexInputAttributeDescription, 2> vulkan_vertex_supporter_internal<vertex_type::POS_TEXTURE>::attributes =
{
    VkVertexInputAttributeDescription { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, (u32)class_offset(vertex_pos_texture, _pos) },
    VkVertexInputAttributeDescription { 1, 0, VK_FORMAT_R32G32_SFLOAT, (u32)class_offset(vertex_pos_texture, _texture) }
};

}

class vulkan_vertex_supporter
{
public:
    static VkPipelineVertexInputStateCreateInfo get_vk_info(vertex_type type)
    {
        switch (type)
        {
        case vertex_type::POS_COLOR:
            return _VkVertexSupportNs::vulkan_vertex_supporter_internal<vertex_type::POS_COLOR>::get_vk_vertex_input_info();

        case vertex_type::POS_TEXTURE:
            return _VkVertexSupportNs::vulkan_vertex_supporter_internal<vertex_type::POS_TEXTURE>::get_vk_vertex_input_info();

        default:
            break;
        }

        assert(0);
        return VkPipelineVertexInputStateCreateInfo();
    }
};

}
