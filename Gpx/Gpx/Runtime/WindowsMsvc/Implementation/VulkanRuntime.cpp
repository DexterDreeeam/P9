
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _instance(nullptr)
{
    AUTO_TRACE;
}

vulkan_runtime::~vulkan_runtime()
{
    AUTO_TRACE;
}

boole vulkan_runtime::init()
{
    AUTO_TRACE;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "p9gpx";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "p9vulkan";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    vector<const char*> extensions;
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    if (_desc.debug_mode)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    vector<const char*> layers;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (_desc.debug_mode)
    {
        layers.push_back("VK_LAYER_KHRONOS_validation");
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = vulkan_runtime::debug_cb;
        createInfo.pNext = &debugCreateInfo;
    }
    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = layers.data();

    auto* ins = new VkInstance();

    if (vkCreateInstance(&createInfo, nullptr, ins) != VK_SUCCESS)
    {
        delete ins;
        return boole::False;
    }

    if (_instance.compare_exchange(nullptr, ins) != nullptr)
    {
        delete ins;
        return boole::False;
    }

    return boole::True;
}

boole vulkan_runtime::uninit()
{
    AUTO_TRACE;

    auto* ins = _instance.exchange(nullptr);
    if (ins)
    {
        vkDestroyInstance(*ins, nullptr);
    }

    return boole::True;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_runtime::debug_cb(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    print("[Vulkan] %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

}