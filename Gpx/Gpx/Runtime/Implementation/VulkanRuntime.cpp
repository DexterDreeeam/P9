
#include "../Interface.hpp"

namespace gpx
{

atom<VkInstance*> vulkan_runtime::_instance = nullptr;

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _self(),
    _vec_window_ctx(),
    _vec_window_ctx_lock()
{
    AUTO_TRACE;

    _vec_window_ctx_lock.init();
}

vulkan_runtime::~vulkan_runtime()
{
    AUTO_TRACE;

    uninit();

    _vec_window_ctx_lock.uninit();
}

void vulkan_runtime::setup_self(obs<runtime> obs_rt)
{
    AUTO_TRACE;
    _self = obs_rt;
}

boole vulkan_runtime::init()
{
    AUTO_TRACE;

    auto* ins = new VkInstance();
    if (_instance.compare_exchange(nullptr, ins) != nullptr)
    {
        delete ins;
        return boole::False;
    }

    glfwInit();

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "p9gpx";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 2, 0);
    appInfo.pEngineName = "p9vulkan";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 2, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    sz_t glfwExtensionsCnt = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions((uint32_t*)&glfwExtensionsCnt);
    vector<const char*> extensions;
    for (s64 i = 0; i < glfwExtensionsCnt; ++i)
    {
        extensions.push_back(glfwExtensions[i]);
    }
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
            //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType =
            //VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
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

    if (vkCreateInstance(&createInfo, nullptr, ins) != VK_SUCCESS)
    {
        delete ins;
        _instance.set(nullptr);
        glfwTerminate();
        return boole::False;
    }

    return boole::True;
}

boole vulkan_runtime::uninit()
{
    AUTO_TRACE;

    auto* ins = _instance.get();
    if (ins == nullptr)
    {
        return boole::False;
    }

    // release all window resource
    for (auto w : _vec_window_ctx)
    {
        w->_window->stop();
    }
    _vec_window_ctx.clear();

    // release vulkan release
    vkDestroyInstance(*ins, nullptr);
    delete ins;
    glfwTerminate();
    _instance.set(nullptr);

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

ref<window_context> vulkan_runtime::get_window_context(const string& window_name)
{
    AUTO_TRACE;

    _vec_window_ctx_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _vec_window_ctx_lock.write_release();
    };

    for (auto itr = _vec_window_ctx.begin(); itr != _vec_window_ctx.end(); ++itr)
    {
        if ((*itr)->_window->name() == window_name)
        {
            return *itr;
        }
    }
    return ref<window_context>();
}

boole vulkan_runtime::window_start_callback(const string& window_name)
{
    AUTO_TRACE;

    auto w_ctx = get_window_context(window_name);
    if (w_ctx.empty())
    {
        return boole::False;
    }

    sz_t device_count = 0;
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, nullptr);

    vector<VkPhysicalDevice> vec_device(device_count);
    vkEnumeratePhysicalDevices(get_vk_instance(), (uint32_t*)&device_count, vec_device.data());

    vector<pair<VkPhysicalDevice, s64>> vec_device_score;
    VkPhysicalDevice target_device = nullptr;

    for (s64 i = 0; i < device_count; ++i)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(vec_device[i], &deviceProperties);
        vkGetPhysicalDeviceFeatures(vec_device[i], &deviceFeatures);

        string device_str = "";
        device_str += deviceProperties.deviceName;
        if (device_str == w_ctx->_preferred_device_name)
        {
            target_device = vec_device[i];
            break;
        }

        s64 score = 0;
        score += deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2000 : 100;
        score += deviceProperties.limits.maxImageDimension2D;
        score = deviceFeatures.geometryShader ? score : 0;
        vec_device_score.push_back(make_pair(vec_device[i], score));
    }

    if (target_device == nullptr)
    {
        if (vec_device_score.size() == 0)
        {
            return boole::False;
        }
        vec_device_score.sort(
            [](pair<VkPhysicalDevice, s64>& p1, pair<VkPhysicalDevice, s64>& p2)
            {
                return p1.second > p2.second;
            }
        );

        for (auto& p : vec_device_score)
        {
            if (build_device_resource(p.first, w_ctx))
            {
                return boole::True;
            }
        }
    }
    else if (build_device_resource(target_device, w_ctx))
    {
        return boole::True;
    }

    return boole::False;
}

boole vulkan_runtime::window_stop_callback(const string& window_name)
{
    AUTO_TRACE;

    auto w_ctx = get_window_context(window_name);
    if (w_ctx.empty())
    {
        return boole::False;
    }

    return clear_device_resource(w_ctx);
}

boole vulkan_runtime::build_device_resource(VkPhysicalDevice physical_device, ref<window_context> w_ctx)
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
    s64 present_queue_family_idx = -1;
    s64 present_queue_family_queue_idx = -1;
    for (s64 loop = 0; loop < 3; ++loop)
    {
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
                else if (present_queue_family_queue_idx < 0)
                {
                    VkBool32 b;
                    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, w_ctx->_window.ref_of<glfw_window>()->_surface, &b);
                    if (b)
                    {
                        present_queue_family_idx = i;
                        present_queue_family_queue_idx = j;
                    }
                }
            }
            if (render_queue_family_queue_idx >= 0 && transfer_queue_family_queue_idx >= 0 && present_queue_family_queue_idx >= 0)
            {
                break;
            }
        }
    }
    if (render_queue_family_idx < 0 || transfer_queue_family_idx < 0 || present_queue_family_idx < 0)
    {
        return boole::False;
    }

    f32 render_queue_priority = 0.7f;
    f32 transfer_queue_priority = 0.5f;
    f32 present_queue_priority = 0.9f;
    vector<VkDeviceQueueCreateInfo> vec_queue_create_info;
    vector<vector<vector<f32>>> vec_vec_queue_priorities;
    for (s64 i = 0; i < queue_families.size(); ++i)
    {
        s64 occupy = 0;
        array<s64, 3> queue_cnts = { 0, 0, 0 };
        vector<vector<f32>> priorities;
        f32* f32_arr = nullptr;
        if (render_queue_family_idx != i && transfer_queue_family_idx != i && present_queue_family_idx != i)
        {
            continue;
        }
        else
        {
            f32_arr = new f32[3];
        }
        if (render_queue_family_idx == i)
        {
            ++queue_cnts[render_queue_family_queue_idx];
            f32_arr[render_queue_family_queue_idx] = render_queue_priority;
        }
        if (transfer_queue_family_idx == i)
        {
            ++queue_cnts[transfer_queue_family_queue_idx];
            f32_arr[transfer_queue_family_queue_idx] = transfer_queue_priority;
        }
        if (present_queue_family_idx == i)
        {
            ++queue_cnts[present_queue_family_queue_idx];
            f32_arr[present_queue_family_queue_idx] = present_queue_priority;
        }
        for (auto c : queue_cnts)
        {
            if (c > 0)
            {
                ++occupy;
            }
        }
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = i;
        queue_create_info.queueCount = occupy;
        queue_create_info.pQueuePriorities = f32_arr;
        vec_queue_create_info.push_back(queue_create_info);
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

    VkDevice logical_device;
    VkResult rst = vkCreateDevice(physical_device, &createInfo, nullptr, &logical_device);

    for (auto& queue_create : vec_queue_create_info)
    {
        delete[] queue_create.pQueuePriorities;
    }

    if (rst != VK_SUCCESS)
    {
        return boole::False;
    }

    w_ctx->_physical_device = physical_device;
    w_ctx->_logical_device = logical_device;

    vkGetDeviceQueue(logical_device, render_queue_family_idx, render_queue_family_queue_idx, &w_ctx->_render_queue);
    vkGetDeviceQueue(logical_device, transfer_queue_family_idx, transfer_queue_family_queue_idx, &w_ctx->_transfer_queue);
    vkGetDeviceQueue(logical_device, present_queue_family_idx, present_queue_family_queue_idx, &w_ctx->_present_queue);
    return boole::True;
}

boole vulkan_runtime::clear_device_resource(ref<window_context> w_ctx)
{
    AUTO_TRACE;

    if (w_ctx->_logical_device)
    {
        vkDestroyDevice(w_ctx->_logical_device, nullptr);
        w_ctx->_physical_device = nullptr;
        w_ctx->_logical_device = nullptr;
        w_ctx->_render_queue = nullptr;
        w_ctx->_transfer_queue = nullptr;
        w_ctx->_present_queue = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

ref<window> vulkan_runtime::build_window(const window_desc& desc, const string& preferred_device_name)
{
    AUTO_TRACE;

    _vec_window_ctx_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _vec_window_ctx_lock.write_release();
    };

    auto wnd = ref<glfw_window>::new_instance(desc, _self);
    if (wnd.empty())
    {
        return ref<window>();
    }

    auto w_ctx = ref<window_context>::new_instance();
    w_ctx->_window = wnd;
    w_ctx->_preferred_device_name = preferred_device_name;
    _vec_window_ctx.push_back(w_ctx);

    return wnd;
}

boole vulkan_runtime::remove_window(const string& window_name)
{
    AUTO_TRACE;

    _vec_window_ctx_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _vec_window_ctx_lock.write_release();
    };

    for (auto itr = _vec_window_ctx.begin(); itr != _vec_window_ctx.end(); ++itr)
    {
        if ((*itr)->_window->name() == window_name)
        {
            _vec_window_ctx.erase(itr);
            return boole::True;
        }
    }
    return boole::False;
}

ref<window> vulkan_runtime::get_window(const string& window_name)
{
    _vec_window_ctx_lock.wait_read();
    escape_function ep = [=]() mutable
    {
        _vec_window_ctx_lock.read_release();
    };

    for (auto w : _vec_window_ctx)
    {
        if (w->_window->name() == window_name)
        {
            return w->_window;
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
