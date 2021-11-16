
#include "../Interface.hpp"

namespace gpx
{

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _self(),
    _vec_window(),
    _vec_window_lock(),
    _instance(nullptr)
{
    AUTO_TRACE;

    _vec_window_lock.init();
}

vulkan_runtime::~vulkan_runtime()
{
    AUTO_TRACE;

    uninit();

    _vec_window_lock.uninit();
}

void vulkan_runtime::setup_self(obs<runtime> obs_rt)
{
    _self = obs_rt;
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
    if (ins == nullptr)
    {
        return boole::False;
    }

    // release all window resource
    for (auto w : _vec_window)
    {
        w->stop();
    }
    _vec_window.clear();

    // release vulkan release
    vkDestroyInstance(*ins, nullptr);
    return boole::True;
}

ref<window> vulkan_runtime::build_window(const window_desc& desc)
{
    _vec_window_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _vec_window_lock.write_release();
    };

    auto wnd = ref<glfw_window>::new_instance(desc, _self).ref_of<window>();
    _vec_window.push_back(wnd);
    return wnd;
}

ref<window> vulkan_runtime::get_window(const string& window_name)
{
    _vec_window_lock.wait_read();
    escape_function ep = [=]() mutable
    {
        _vec_window_lock.read_release();
    };

    for (auto w : _vec_window)
    {
        if (w->name() == window_name)
        {
            return w;
        }
    }
    return ref<window>();
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
