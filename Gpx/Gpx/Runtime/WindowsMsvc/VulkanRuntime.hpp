#pragma once

#include "../../../External/Vulkan/Windows/Header/vulkan/vulkan.h"

#include "../Interface.hpp"

namespace gpx
{

class vulkan_runtime : public runtime
{
public:
    vulkan_runtime(const runtime_desc& desc);

    virtual ~vulkan_runtime() override;

public:
    virtual boole init() override;

    virtual boole uninit() override;

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

private:
    runtime_desc       _desc;
    atom<VkInstance*>  _instance;
};

}
