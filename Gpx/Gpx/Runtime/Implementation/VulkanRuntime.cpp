
#include "../Interface.hpp"
#include "../VulkanRuntime.hpp"
#include "../VulkanPipeline.hpp"
#include "../VulkanShader.hpp"
#include "../../Window/GlfwWindow.hpp"

namespace gpx
{

atom<VkInstance*> vulkan_runtime::_instance = nullptr;

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _self(),
    _window_ctx_vec(),
    _window_ctx_vec_lock(),
    _pipeline_map(),
    _pipeline_map_lock()
{
    AUTO_TRACE;

    _window_ctx_vec_lock.init();
    _pipeline_map_lock.init();
}

vulkan_runtime::~vulkan_runtime()
{
    AUTO_TRACE;

    uninit();

    _pipeline_map_lock.uninit();
    _window_ctx_vec_lock.uninit();
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
    for (auto w : _window_ctx_vec)
    {
        w->_window->stop();
    }
    _window_ctx_vec.clear();

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

ref<vulkan_window_context> vulkan_runtime::get_window_context(const string& window_name)
{
    AUTO_TRACE;

    _window_ctx_vec_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _window_ctx_vec_lock.write_release();
    };

    for (auto itr = _window_ctx_vec.begin(); itr != _window_ctx_vec.end(); ++itr)
    {
        if ((*itr)->_window->name() == window_name)
        {
            return *itr;
        }
    }
    return ref<vulkan_window_context>();
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
            if (build_device_hardware_resource(p.first, w_ctx) && build_device_image_resource(w_ctx))
            {
                return boole::True;
            }
        }
    }
    else if (build_device_hardware_resource(target_device, w_ctx) && build_device_image_resource(w_ctx))
    {
        return boole::True;
    }

    clear_device_resource(w_ctx);
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

boole vulkan_runtime::build_device_hardware_resource(VkPhysicalDevice physical_device, ref<vulkan_window_context> w_ctx)
{
    AUTO_TRACE;
    assert(physical_device);

    // find right queue family
    // find right queue index
    // create logical device

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
    vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = vec_queue_create_info.data();
    createInfo.queueCreateInfoCount = vec_queue_create_info.size();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    createInfo.enabledLayerCount = _vec_validation_layer.size();
    createInfo.ppEnabledLayerNames = _vec_validation_layer.data();

    VkDevice logical_device;
    VkResult vk_rst = vkCreateDevice(physical_device, &createInfo, nullptr, &logical_device);

    for (auto& queue_create : vec_queue_create_info)
    {
        delete[] queue_create.pQueuePriorities;
    }

    if (vk_rst != VK_SUCCESS)
    {
        return boole::False;
    }

    w_ctx->_physical_device = physical_device;
    w_ctx->_logical_device = logical_device;

    w_ctx->_render_queue_family_idx = render_queue_family_idx;
    w_ctx->_transfer_queue_family_idx = transfer_queue_family_idx;
    w_ctx->_present_queue_family_idx = present_queue_family_idx;
    vkGetDeviceQueue(logical_device, render_queue_family_idx, render_queue_family_queue_idx, &w_ctx->_render_queue);
    vkGetDeviceQueue(logical_device, transfer_queue_family_idx, transfer_queue_family_queue_idx, &w_ctx->_transfer_queue);
    vkGetDeviceQueue(logical_device, present_queue_family_idx, present_queue_family_queue_idx, &w_ctx->_present_queue);

    return boole::True;
}

boole vulkan_runtime::build_device_image_resource(ref<vulkan_window_context> w_ctx)
{
    VkSurfaceKHR surface = w_ctx->_window.ref_of<glfw_window>()->_surface;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        w_ctx->_physical_device, surface, &surfaceCapabilities);

    // surface format
    boole is_format_chosen = boole::False;
    sz_t format_count = 0;
    vector<VkSurfaceFormatKHR> vec_format;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        w_ctx->_physical_device, surface, (uint32_t*)&format_count, nullptr);
    if (format_count == 0)
    {
        return boole::False;
    }
    vec_format = vector<VkSurfaceFormatKHR>(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        w_ctx->_physical_device, surface, (uint32_t*)&format_count, vec_format.data());
    for (auto& f : vec_format)
    {
        if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            w_ctx->_surface_format = f;
            is_format_chosen = boole::True;
        }
    }
    if (!is_format_chosen)
    {
        w_ctx->_surface_format = vec_format[0];
    }

    // present mode
    boole is_present_mode_chosen = boole::False;
    sz_t present_mode_count = 0;
    vector<VkPresentModeKHR> vec_present_mode;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        w_ctx->_physical_device, surface, (uint32_t*)&present_mode_count, nullptr);
    if (present_mode_count == 0)
    {
        return boole::False;
    }
    vec_present_mode = vector<VkPresentModeKHR>(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        w_ctx->_physical_device, surface, (uint32_t*)&present_mode_count, vec_present_mode.data());
    for (auto& pm : vec_present_mode)
    {
        if (pm == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            w_ctx->_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            is_present_mode_chosen = boole::True;
        }
    }
    if (!is_present_mode_chosen)
    {
        w_ctx->_present_mode = VK_PRESENT_MODE_FIFO_KHR;
    }

    // swap chain extent
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
        w_ctx->_swap_chain_extent = surfaceCapabilities.currentExtent;
    }
    else
    {
        u64 width = 0;
        u64 height = 0;
        glfwGetFramebufferSize(w_ctx->_window.ref_of<glfw_window>()->_ctx, (int*)&width, (int*)&height);
        VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };
        actualExtent.width = math::clamp(actualExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        actualExtent.height = math::clamp(actualExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
        w_ctx->_swap_chain_extent = actualExtent;
    }

    // swap chain image count
    u64 preferred_image_count = 3;
    preferred_image_count = math::clamp(preferred_image_count, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);

    // swap chain
    uint32_t queueFamilyIndices[] = { (uint32_t)w_ctx->_render_queue_family_idx, (uint32_t)w_ctx->_present_queue_family_idx };
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = preferred_image_count;
    createInfo.imageFormat = w_ctx->_surface_format.format;
    createInfo.imageColorSpace = w_ctx->_surface_format.colorSpace;
    createInfo.imageExtent = w_ctx->_swap_chain_extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT for post-processing
    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = w_ctx->_present_mode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    if (vkCreateSwapchainKHR(w_ctx->_logical_device, &createInfo, nullptr, &w_ctx->_swap_chain) != VK_SUCCESS)
    {
        return boole::False;
    }

    // swap chain image
    sz_t image_count = 0;
    vkGetSwapchainImagesKHR(w_ctx->_logical_device, w_ctx->_swap_chain, (uint32_t*)&image_count, nullptr);
    w_ctx->_swap_chain_image_vec = vector<VkImage>(image_count);
    if (vkGetSwapchainImagesKHR(
        w_ctx->_logical_device, w_ctx->_swap_chain, (uint32_t*)&image_count, w_ctx->_swap_chain_image_vec.data()) != VK_SUCCESS)
    {
        clear_device_resource(w_ctx);
        return boole::False;
    }

    // image view
    w_ctx->_image_view_vec = vector<VkImageView>(image_count);
    for (s64 i = 0; i < image_count; ++i)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = w_ctx->_swap_chain_image_vec[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = w_ctx->_surface_format.format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(w_ctx->_logical_device, &createInfo, nullptr, &w_ctx->_image_view_vec[i]) != VK_SUCCESS)
        {
            clear_device_resource(w_ctx);
            return boole::False;
        }
    }

    return boole::True;
}

boole vulkan_runtime::clear_device_resource(ref<vulkan_window_context> w_ctx)
{
    AUTO_TRACE;
    while (w_ctx->_image_view_vec.size())
    {
        vkDestroyImageView(w_ctx->_logical_device, w_ctx->_image_view_vec.back(), nullptr);
        w_ctx->_image_view_vec.pop_back();
    }
    if (w_ctx->_swap_chain)
    {
        vkDestroySwapchainKHR(w_ctx->_logical_device, w_ctx->_swap_chain, nullptr);
        w_ctx->_swap_chain = nullptr;
    }
    if (w_ctx->_logical_device)
    {
        vkDestroyDevice(w_ctx->_logical_device, nullptr);
        w_ctx->_physical_device = nullptr;
        w_ctx->_logical_device = nullptr;
        w_ctx->_render_queue = nullptr;
        w_ctx->_transfer_queue = nullptr;
        w_ctx->_present_queue = nullptr;
    }
    return boole::True;
}

ref<window> vulkan_runtime::build_window(const window_desc& desc, const string& preferred_device_name)
{
    AUTO_TRACE;

    _window_ctx_vec_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _window_ctx_vec_lock.write_release();
    };

    auto wnd = ref<glfw_window>::new_instance(desc, _self);
    if (wnd.empty())
    {
        return ref<window>();
    }

    auto w_ctx = ref<vulkan_window_context>::new_instance();
    w_ctx->_window = wnd;
    w_ctx->_preferred_device_name = preferred_device_name;
    w_ctx->_logical_device = nullptr;
    w_ctx->_swap_chain = nullptr;
    _window_ctx_vec.push_back(w_ctx);

    return wnd;
}

boole vulkan_runtime::remove_window(const string& window_name)
{
    AUTO_TRACE;

    _window_ctx_vec_lock.wait_write();
    escape_function ep = [=]() mutable
    {
        _window_ctx_vec_lock.write_release();
    };

    for (auto itr = _window_ctx_vec.begin(); itr != _window_ctx_vec.end(); ++itr)
    {
        if ((*itr)->_window->name() == window_name)
        {
            _window_ctx_vec.erase(itr);
            return boole::True;
        }
    }
    return boole::False;
}

ref<window> vulkan_runtime::get_window(const string& window_name)
{
    _window_ctx_vec_lock.wait_read();
    escape_function ep = [=]() mutable
    {
        _window_ctx_vec_lock.read_release();
    };

    for (auto w : _window_ctx_vec)
    {
        if (w->_window->name() == window_name)
        {
            return w->_window;
        }
    }
    return ref<window>();
}

ref<shader> vulkan_runtime::build_shader(const shader_desc& desc)
{
    auto w_ctx = get_window_context(desc._window_name);
    if (w_ctx.empty())
    {
        return ref<shader>();
    }

    auto r_shader = ref<vulkan_shader>::new_instance(desc, w_ctx);
    if (!r_shader->load(desc._shader_path))
    {
        return ref<shader>();
    }
    return r_shader;
}

boole vulkan_runtime::register_pipeline(const pipeline_desc& desc)
{
    auto w_ctx = get_window_context(desc._window_name);
    if (w_ctx.empty())
    {
        return boole::False;
    }

    auto r_pipeline = ref<vulkan_pipeline>::new_instance(w_ctx);
    if (!r_pipeline->init(desc))
    {
        return boole::False;
    }

    if (!_pipeline_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _pipeline_map_lock.write_release();
    };

    if (_pipeline_map.find(desc._pipeline_name) != _pipeline_map.end())
    {
        // already has a same name pipeline
        return boole::False;
    }

    _pipeline_map[desc._pipeline_name] = r_pipeline;
    return boole::True;
}

ref<pipeline> vulkan_runtime::get_pipeline(const string& pipeline_name)
{
    ref<pipeline> ret;

    if (!_pipeline_map_lock.wait_read())
    {
        return ret;
    }
    escape_function ef =
        [=]() mutable
    {
        _pipeline_map_lock.read_release();
    };

    auto itr = _pipeline_map.find(pipeline_name);
    if (itr != _pipeline_map.end())
    {
        ret = itr->second;
    }
    return ret;
}

boole vulkan_runtime::unregister_pipeline(const string& pipeline_name)
{
    if (!_pipeline_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _pipeline_map_lock.write_release();
    };

    return _pipeline_map.erase(pipeline_name);
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
