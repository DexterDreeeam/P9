
#include "../Interface.hpp"

namespace gpx
{

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _self(),
    _instance(nullptr),
    _physical_device(nullptr),
    _logical_device(nullptr),
    _render_queue(nullptr),
    _transfer_queue(nullptr),
    _vec_window(),
    _vec_window_lock()
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
    AUTO_TRACE;
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

    sz_t glfwExtensionsCnt = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions((uint32_t*)&glfwExtensionsCnt);
    vector<const char*> extensions;
    for (s64 i = 0; i < glfwExtensionsCnt; ++i)
    {
        extensions.push_back(glfwExtensions[i]);
    }
    // extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    if (_desc.debug_mode)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (_desc.debug_mode)
    {
        _vec_validation_layer.push_back("VK_LAYER_KHRONOS_validation");
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
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = _vec_validation_layer.size();
    createInfo.ppEnabledLayerNames = _vec_validation_layer.data();
    createInfo.pNext = _desc.debug_mode ? &debugCreateInfo : nullptr;

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

    // release device resource
    clear_device_resource();

    // release all window resource
    for (auto w : _vec_window)
    {
        w->stop();
    }
    _vec_window.clear();

    // release vulkan release
    vkDestroyInstance(*ins, nullptr);
    delete ins;
    return boole::True;
}

vector<string> vulkan_runtime::list_device()
{
    AUTO_TRACE;
    assert(get_vk_instance());

    vector<string> rst;

    sz_t device_count = 0;
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, nullptr);

    vector<VkPhysicalDevice> vec_device(device_count);
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, vec_device.data());

    for (auto& dev : vec_device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(dev, &deviceProperties);

        string device_str = "";
        device_str += deviceProperties.deviceName;
        rst.push_back(device_str);
    }
    return rst;
}

boole vulkan_runtime::select_device(const string& device_name)
{
    AUTO_TRACE;

    clear_device_resource();

    VkPhysicalDevice target_device = nullptr;
    s64 best_device_score = 0;
    s64 best_device_idx = -1;

    sz_t device_count = 0;
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, nullptr);

    vector<VkPhysicalDevice> vec_device(device_count);
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, vec_device.data());

    for (s64 i = 0; i < device_count; ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(vec_device[i], &deviceProperties);
        vkGetPhysicalDeviceFeatures(vec_device[i], &deviceFeatures);

        string device_str = "";
        device_str += deviceProperties.deviceName;
        if (device_str == device_name)
        {
            target_device = vec_device[i];
            break;
        }

        s64 score = 0;
        score += deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2000 : 100;
        score += deviceProperties.limits.maxImageDimension2D;
        score = deviceFeatures.geometryShader ? score : 0;

        best_device_idx = score > best_device_score ? i : best_device_idx;
    }

    if (target_device == nullptr && best_device_idx < 0)
    {
        return boole::False;
    }
    if (target_device == nullptr)
    {
        target_device = vec_device[best_device_idx];
    }
    if (build_device_resource(target_device))
    {
        _physical_device = target_device;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole vulkan_runtime::build_device_resource(VkPhysicalDevice physical_device)
{
    AUTO_TRACE;
    assert(physical_device);

    sz_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, (uint32_t*)&queue_family_count, nullptr);

    vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, (uint32_t*)&queue_family_count, queue_families.data());

    s64 render_queue_family_idx = -1;
    s64 render_queue_family_queue_idx = -1;
    s64 transfer_queue_family_idx = -1;
    s64 transfer_queue_family_queue_idx = -1;
    for (s64 i = 0; i < queue_families.size(); ++i)
    {
        auto f = queue_families[i];
        for (s64 j = 0; j < f.queueCount; ++j)
        {
            if (render_queue_family_queue_idx < 0 && f.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                render_queue_family_idx = i;
                render_queue_family_queue_idx = j;
            }
            else if (transfer_queue_family_queue_idx < 0 && f.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                transfer_queue_family_idx = i;
                transfer_queue_family_queue_idx = j;
            }
        }
        if (render_queue_family_queue_idx >= 0 && transfer_queue_family_queue_idx >= 0)
        {
            break;
        }
    }
    assert(render_queue_family_idx >= 0 && transfer_queue_family_idx >= 0);

    f32 render_queue_priority = 1.0f;
    f32 transfer_queue_priority = 0.5f;
    vector<f32> priorities = { render_queue_priority, transfer_queue_priority };
    vector<VkDeviceQueueCreateInfo> vec_queue_create_info;
    if (render_queue_family_idx == transfer_queue_family_idx)
    {
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = render_queue_family_idx;
        queue_create_info.queueCount = 2;
        queue_create_info.pQueuePriorities = priorities.data();

        vec_queue_create_info.push_back(queue_create_info);
    }
    else
    {
        VkDeviceQueueCreateInfo render_queue_create_info = {};
        render_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        render_queue_create_info.queueFamilyIndex = render_queue_family_idx;
        render_queue_create_info.queueCount = 1;
        render_queue_create_info.pQueuePriorities = &render_queue_priority;

        VkDeviceQueueCreateInfo transfer_queue_create_info = {};
        transfer_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        transfer_queue_create_info.queueFamilyIndex = transfer_queue_family_idx;
        transfer_queue_create_info.queueCount = 1;
        transfer_queue_create_info.pQueuePriorities = &render_queue_priority;

        vec_queue_create_info.push_back(render_queue_create_info);
        vec_queue_create_info.push_back(transfer_queue_create_info);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = vec_queue_create_info.data();
    createInfo.queueCreateInfoCount = vec_queue_create_info.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = _vec_validation_layer.size();
    createInfo.ppEnabledLayerNames = _vec_validation_layer.data();
    if (vkCreateDevice(physical_device, &createInfo, nullptr, &_logical_device) != VK_SUCCESS)
    {
        return boole::False;
    }

    vkGetDeviceQueue(_logical_device, render_queue_family_idx, render_queue_family_queue_idx, &_render_queue);
    vkGetDeviceQueue(_logical_device, render_queue_family_idx, render_queue_family_queue_idx, &_transfer_queue);
    return boole::True;
}

boole vulkan_runtime::clear_device_resource()
{
    AUTO_TRACE;

    vkDestroyDevice(_logical_device, nullptr);
    return boole::True;
}

ref<window> vulkan_runtime::build_window(const window_desc& desc)
{
    AUTO_TRACE;

    _vec_window_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _vec_window_lock.write_release();
    };

    auto wnd = ref<glfw_window>::new_instance(desc, _self);
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
