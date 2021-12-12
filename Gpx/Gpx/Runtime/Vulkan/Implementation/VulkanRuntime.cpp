
#include "../VulkanInterface.hpp"

namespace gpx
{

atom<VkInstance*> vulkan_runtime::_instance = nullptr;

vulkan_runtime::vulkan_runtime(const runtime_desc& desc) :
    runtime(),
    _desc(desc),
    _self(),
    _resource_lk(),
    _preferred_device_name(),
    _physical_device(nullptr),
    _logical_device(nullptr),
    _msaa(VK_SAMPLE_COUNT_1_BIT),
    _render_queue_family_idx(-1),
    _present_queue_family_idx(-1),
    _transfer_queue_family_idx(-1),
    _render_queue(nullptr),
    _present_queue(nullptr),
    _transfer_queue(nullptr),
    _render_command_pool(nullptr),
    _transfer_command_pool(nullptr),
    _window_ctx_vec(),
    _window_ctx_vec_lock(),
    _pipeline_map(),
    _pipeline_map_lock(),
    _vertices_viewer_map(),
    _vertices_viewer_map_lock(),
    _texture_viewer_map(),
    _texture_viewer_map_lock(),
    _dynamic_memory_map(),
    _dynamic_memory_map_lock()
{
    AUTO_TRACE;

    _resource_lk.init();
    _window_ctx_vec_lock.init();
    _pipeline_map_lock.init();
    _vertices_viewer_map_lock.init();
    _texture_viewer_map_lock.init();
    _dynamic_memory_map_lock.init();
}

vulkan_runtime::~vulkan_runtime()
{
    AUTO_TRACE;

    uninit();

    _dynamic_memory_map_lock.uninit();
    _texture_viewer_map_lock.uninit();
    _vertices_viewer_map_lock.uninit();
    _pipeline_map_lock.uninit();
    _window_ctx_vec_lock.uninit();
    _resource_lk.uninit();
}

void vulkan_runtime::setup_self(obs<runtime> obs_rt)
{
    AUTO_TRACE;
    _self = obs_rt.obs_of<vulkan_runtime>();
}

boole vulkan_runtime::init(const string& preferred_device_name)
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

    _preferred_device_name = preferred_device_name;
    return boole::True;
}

boole vulkan_runtime::uninit()
{
    AUTO_TRACE;

    clear_device_hardware_resource();
    _preferred_device_name = "";

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
    escape_function ep =
        [=]() mutable
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

ref<vulkan_vertices_viewer> vulkan_runtime::get_vertices_viewer(const string& vertices_viewer)
{
    ref<vulkan_vertices_viewer> ret;

    if (!_vertices_viewer_map_lock.wait_read())
    {
        return ret;
    }
    escape_function ef =
        [=]() mutable
    {
        _vertices_viewer_map_lock.read_release();
    };

    auto itr = _vertices_viewer_map.find(vertices_viewer);
    if (itr != _vertices_viewer_map.end())
    {
        ret = itr->second;
    }
    return ret;
}

ref<vulkan_texture_viewer> vulkan_runtime::get_texture_viewer(const string& texture_viewer)
{
    ref<vulkan_texture_viewer> ret;

    if (!_texture_viewer_map_lock.wait_read())
    {
        return ret;
    }
    escape_function ef =
        [=]() mutable
    {
        _texture_viewer_map_lock.read_release();
    };

    auto itr = _texture_viewer_map.find(texture_viewer);
    if (itr != _texture_viewer_map.end())
    {
        ret = itr->second;
    }
    return ret;
}

ref<vulkan_dynamic_memory> vulkan_runtime::get_dynamic_memory(const string& dynamic_memory)
{
    if (!_dynamic_memory_map_lock.wait_read())
    {
        return ref<vulkan_dynamic_memory>();
    }
    escape_function ef =
        [=]() mutable
    {
        _dynamic_memory_map_lock.read_release();
    };

    auto itr = _dynamic_memory_map.find(dynamic_memory);
    if (itr != _dynamic_memory_map.end())
    {
        return itr->second;
    }
    return ref<vulkan_dynamic_memory>();
}

ref<vulkan_pipeline> vulkan_runtime::get_pipeline(const string& pipeline_name)
{
    ref<vulkan_pipeline> ret;

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

boole vulkan_runtime::window_start_callback(const string& window_name)
{
    AUTO_TRACE;

    auto w_ctx = get_window_context(window_name);
    if (w_ctx.empty())
    {
        return boole::False;
    }

    _resource_lk.wait_acquire();
    escape_function ef_lk =
        [=]() mutable
        {
            _resource_lk.release();
        };

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
        if (device_str == _preferred_device_name)
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
            if (build_device_hardware_resource(p.first, w_ctx), build_device_image_resource(w_ctx))
            {
                return boole::True;
            }
        }
    }
    else if (build_device_hardware_resource(target_device, w_ctx) && build_device_image_resource(w_ctx))
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

    return clear_device_image_resource(w_ctx);
}

boole vulkan_runtime::build_device_hardware_resource(VkPhysicalDevice physical_device, ref<vulkan_window_context> w_ctx)
{
    AUTO_TRACE;
    assert(physical_device);

    if (_logical_device)
    {
        return boole::True;
    }

    // find right msaa flag
    // find right queue family
    // find right queue index
    // create logical device
    // create command pool

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physical_device, &physicalDeviceProperties);
    VkSampleCountFlags msaa_limit =
        physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    s64 msaa_level = math::clamp(_desc.msaa_level, 0, 6);
    VkSampleCountFlagBits target_msaa = VK_SAMPLE_COUNT_1_BIT;
    while (1)
    {
        VkSampleCountFlagBits next_msaa = (VkSampleCountFlagBits)((u64)target_msaa << 1);
        if (msaa_level > 0 && (msaa_limit & next_msaa))
        {
            target_msaa = next_msaa;
            --msaa_level;
            continue;
        }
        break;
    }
    _msaa = target_msaa;

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
        clear_device_hardware_resource();
        return boole::False;
    }

    VkCommandPool render_cp;
    VkCommandPool transfer_cp;
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    poolInfo.queueFamilyIndex = render_queue_family_idx;
    if (vkCreateCommandPool(logical_device, &poolInfo, nullptr, &render_cp) != VK_SUCCESS)
    {
        clear_device_hardware_resource();
        return boole::False;
    }
    poolInfo.queueFamilyIndex = transfer_queue_family_idx;
    if (vkCreateCommandPool(logical_device, &poolInfo, nullptr, &transfer_cp) != VK_SUCCESS)
    {
        clear_device_hardware_resource();
        return boole::False;
    }

    _physical_device = physical_device;
    _logical_device = logical_device;

    _render_queue_family_idx = render_queue_family_idx;
    _transfer_queue_family_idx = transfer_queue_family_idx;
    _present_queue_family_idx = present_queue_family_idx;
    vkGetDeviceQueue(logical_device, render_queue_family_idx, render_queue_family_queue_idx, &_render_queue);
    vkGetDeviceQueue(logical_device, transfer_queue_family_idx, transfer_queue_family_queue_idx, &_transfer_queue);
    vkGetDeviceQueue(logical_device, present_queue_family_idx, present_queue_family_queue_idx, &_present_queue);

    _render_command_pool = render_cp;
    _transfer_command_pool = transfer_cp;

    return boole::True;
}

boole vulkan_runtime::build_device_image_resource(ref<vulkan_window_context> w_ctx)
{
    AUTO_TRACE;

    escape_function ef_error_clear_resource =
        [=]() mutable
        {
            clear_device_image_resource(w_ctx);
        };

    VkSurfaceKHR surface = w_ctx->_window.ref_of<glfw_window>()->_surface;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        _physical_device, surface, &surfaceCapabilities);

    // surface format
    boole is_format_chosen = boole::False;
    sz_t format_count = 0;
    vector<VkSurfaceFormatKHR> vec_format;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        _physical_device, surface, (uint32_t*)&format_count, nullptr);
    if (format_count == 0)
    {
        return boole::False;
    }
    vec_format = vector<VkSurfaceFormatKHR>(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        _physical_device, surface, (uint32_t*)&format_count, vec_format.data());
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
        _physical_device, surface, (uint32_t*)&present_mode_count, nullptr);
    if (present_mode_count == 0)
    {
        return boole::False;
    }
    vec_present_mode = vector<VkPresentModeKHR>(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        _physical_device, surface, (uint32_t*)&present_mode_count, vec_present_mode.data());
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
    _desc.frame_count = math::clamp(_desc.frame_count, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);

    // swap chain
    uint32_t queueFamilyIndices[] = { (uint32_t)_render_queue_family_idx, (uint32_t)_present_queue_family_idx };
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = _desc.frame_count;
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
    if (vkCreateSwapchainKHR(_logical_device, &createInfo, nullptr, &w_ctx->_swap_chain) != VK_SUCCESS)
    {
        return boole::False;
    }

    // swap chain image
    sz_t image_count = 0;
    vkGetSwapchainImagesKHR(_logical_device, w_ctx->_swap_chain, (uint32_t*)&image_count, nullptr);
    w_ctx->_swap_chain_image_vec = vector<VkImage>(image_count);
    if (vkGetSwapchainImagesKHR(
        _logical_device, w_ctx->_swap_chain, (uint32_t*)&image_count, w_ctx->_swap_chain_image_vec.data()) != VK_SUCCESS)
    {
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

        if (vkCreateImageView(_logical_device, &createInfo, nullptr, &w_ctx->_image_view_vec[i]) != VK_SUCCESS)
        {
            return boole::False;
        }
    }

    // msaa image
    if (_msaa != VK_SAMPLE_COUNT_1_BIT)
    {
        VkImageCreateInfo msaaImageInfo = {};
        msaaImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        msaaImageInfo.imageType = VK_IMAGE_TYPE_2D;
        msaaImageInfo.extent.width = w_ctx->_swap_chain_extent.width;
        msaaImageInfo.extent.height = w_ctx->_swap_chain_extent.height;
        msaaImageInfo.extent.depth = 1;
        msaaImageInfo.mipLevels = 1;
        msaaImageInfo.arrayLayers = 1;
        msaaImageInfo.format = w_ctx->_surface_format.format;
        msaaImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        msaaImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        msaaImageInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        msaaImageInfo.samples = _msaa;
        msaaImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        for (s64 i = 0; i < image_count; ++i)
        {
            VkImage msaa_image;
            if (vkCreateImage(_logical_device, &msaaImageInfo, nullptr, &msaa_image) != VK_SUCCESS)
            {
                return boole::False;
            }
            w_ctx->_msaa_image_vec.push_back(msaa_image);
        }

        VkMemoryRequirements msaa_memory_requirements;
        vkGetImageMemoryRequirements(_logical_device, w_ctx->_msaa_image_vec[0], &msaa_memory_requirements);

        VkMemoryAllocateInfo msaaAllocInfo = {};
        msaaAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        msaaAllocInfo.allocationSize = msaa_memory_requirements.size;
        msaaAllocInfo.memoryTypeIndex = get_vk_memory_type(_physical_device, msaa_memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        for (s64 i = 0; i < image_count; ++i)
        {
            VkDeviceMemory msaa_memory;
            if (vkAllocateMemory(_logical_device, &msaaAllocInfo, nullptr, &msaa_memory) != VK_SUCCESS)
            {
                return boole::False;
            }
            w_ctx->_msaa_image_memory_vec.push_back(msaa_memory);
            vkBindImageMemory(_logical_device, w_ctx->_msaa_image_vec[i], msaa_memory, 0);

            VkImageViewCreateInfo msaaViewInfo = {};
            msaaViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            msaaViewInfo.image = w_ctx->_msaa_image_vec[i];
            msaaViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            msaaViewInfo.format = w_ctx->_surface_format.format;
            msaaViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            msaaViewInfo.subresourceRange.baseMipLevel = 0;
            msaaViewInfo.subresourceRange.levelCount = 1;
            msaaViewInfo.subresourceRange.baseArrayLayer = 0;
            msaaViewInfo.subresourceRange.layerCount = 1;

            VkImageView msaa_view;
            if (vkCreateImageView(_logical_device, &msaaViewInfo, nullptr, &msaa_view) != VK_SUCCESS)
            {
                return boole::False;
            }
            w_ctx->_msaa_image_view_vec.push_back(msaa_view);

            vulkan_runtime::convert_vk_image_layout(
                _logical_device, _transfer_command_pool, _transfer_queue, w_ctx->_msaa_image_vec[i], 4);
        }
    }

    // depth image
    vector<VkFormat> depth_format_candidate = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT };
    VkFormat depth_format = VK_FORMAT_UNDEFINED;
    for (auto fmt : depth_format_candidate)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_physical_device, fmt, &props);
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            depth_format = fmt;
            break;
        }
    }
    if (depth_format == VK_FORMAT_UNDEFINED)
    {
        return boole::False;
    }
    w_ctx->_depth_format = depth_format;

    VkImageCreateInfo depthInfo = {};
    depthInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    depthInfo.imageType = VK_IMAGE_TYPE_2D;
    depthInfo.extent.width = w_ctx->_swap_chain_extent.width;
    depthInfo.extent.height = w_ctx->_swap_chain_extent.height;
    depthInfo.extent.depth = 1;
    depthInfo.mipLevels = 1;
    depthInfo.arrayLayers = 1;
    depthInfo.format = depth_format;
    depthInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    depthInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depthInfo.samples = _msaa;
    depthInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    for (s64 i = 0; i < image_count; ++i)
    {
        VkImage depth_image;
        if (vkCreateImage(_logical_device, &depthInfo, nullptr, &depth_image) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_depth_image_vec.push_back(depth_image);
    }

    VkMemoryRequirements depth_memory_requirements;
    vkGetImageMemoryRequirements(_logical_device, w_ctx->_depth_image_vec[0], &depth_memory_requirements);

    VkMemoryAllocateInfo depthAllocInfo = {};
    depthAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    depthAllocInfo.allocationSize  = depth_memory_requirements.size;
    depthAllocInfo.memoryTypeIndex = get_vk_memory_type(_physical_device, depth_memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    for (s64 i = 0; i < image_count; ++i)
    {
        VkDeviceMemory depth_memory;
        if (vkAllocateMemory(_logical_device, &depthAllocInfo, nullptr, &depth_memory) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_depth_image_memory_vec.push_back(depth_memory);
        vkBindImageMemory(_logical_device, w_ctx->_depth_image_vec[i], depth_memory, 0);

        VkImageViewCreateInfo depthViewInfo = {};
        depthViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        depthViewInfo.image = w_ctx->_depth_image_vec[i];
        depthViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        depthViewInfo.format = depth_format;
        depthViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        depthViewInfo.subresourceRange.baseMipLevel = 0;
        depthViewInfo.subresourceRange.levelCount = 1;
        depthViewInfo.subresourceRange.baseArrayLayer = 0;
        depthViewInfo.subresourceRange.layerCount = 1;

        VkImageView depth_view;
        if (vkCreateImageView(_logical_device, &depthViewInfo, nullptr, &depth_view) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_depth_image_view_vec.push_back(depth_view);

        vulkan_runtime::convert_vk_image_layout(
            _logical_device, _transfer_command_pool, _transfer_queue, w_ctx->_depth_image_vec[i], 3);
    }

    // semaphore & fence
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (s64 i = 0; i < image_count; ++i)
    {
        VkSemaphore sema;
        VkFence fence;
        if (vkCreateSemaphore(_logical_device, &semaphoreInfo, nullptr, &sema) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_image_available_sema_vec.push_back(sema);
        if (vkCreateSemaphore(_logical_device, &semaphoreInfo, nullptr, &sema) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_render_complete_sema_vec.push_back(sema);
        if (vkCreateFence(_logical_device, &fenceInfo, nullptr, &fence) != VK_SUCCESS)
        {
            return boole::False;
        }
        w_ctx->_inflight_fence_vec.push_back(fence);
    }
    w_ctx->_reordered_fence_vec.clear();
    w_ctx->_reordered_fence_vec.resize(w_ctx->_swap_chain_image_vec.size(), nullptr);
    w_ctx->_fence_counter = 0;
    ef_error_clear_resource.disable();
    return boole::True;
}

boole vulkan_runtime::clear_device_hardware_resource()
{
    AUTO_TRACE;

    _resource_lk.wait_acquire();
    escape_function ef_lk =
        [=]() mutable
        {
            _resource_lk.release();
        };

    if (_render_command_pool)
    {
        vkDestroyCommandPool(_logical_device, _render_command_pool, nullptr);
        _render_command_pool = nullptr;
    }
    if (_transfer_command_pool)
    {
        vkDestroyCommandPool(_logical_device, _transfer_command_pool, nullptr);
        _transfer_command_pool = nullptr;
    }
    if (_logical_device)
    {
        vkDestroyDevice(_logical_device, nullptr);
        _physical_device = nullptr;
        _logical_device = nullptr;
        _render_queue = nullptr;
        _transfer_queue = nullptr;
        _present_queue = nullptr;
        _render_queue_family_idx = -1;
        _present_queue_family_idx = -1;
        _transfer_queue_family_idx = -1;
    }
    return boole::True;
}

boole vulkan_runtime::clear_device_image_resource(ref<vulkan_window_context> w_ctx)
{
    AUTO_TRACE;

    _resource_lk.wait_acquire();
    escape_function ef_lk =
        [=]() mutable
        {
            _resource_lk.release();
        };

    if (w_ctx.empty())
    {
        return boole::False;
    }

    while (w_ctx->_image_available_sema_vec.size())
    {
        vkDestroySemaphore(_logical_device, w_ctx->_image_available_sema_vec.back(), nullptr);
        w_ctx->_image_available_sema_vec.pop_back();
    }
    while (w_ctx->_render_complete_sema_vec.size())
    {
        vkDestroySemaphore(_logical_device, w_ctx->_render_complete_sema_vec.back(), nullptr);
        w_ctx->_render_complete_sema_vec.pop_back();
    }
    while (w_ctx->_inflight_fence_vec.size())
    {
        vkDestroyFence(_logical_device, w_ctx->_inflight_fence_vec.back(), nullptr);
        w_ctx->_inflight_fence_vec.pop_back();
    }
    w_ctx->_reordered_fence_vec.clear();
    w_ctx->_fence_counter = 0;
    while (w_ctx->_msaa_image_view_vec.size())
    {
        vkDestroyImageView(_logical_device, w_ctx->_msaa_image_view_vec.back(), nullptr);
        w_ctx->_msaa_image_view_vec.pop_back();
    }
    while (w_ctx->_msaa_image_memory_vec.size())
    {
        vkFreeMemory(_logical_device, w_ctx->_msaa_image_memory_vec.back(), nullptr);
        w_ctx->_msaa_image_memory_vec.pop_back();
    }
    while (w_ctx->_msaa_image_vec.size())
    {
        vkDestroyImage(_logical_device, w_ctx->_msaa_image_vec.back(), nullptr);
        w_ctx->_msaa_image_vec.pop_back();
    }
    while (w_ctx->_depth_image_view_vec.size())
    {
        vkDestroyImageView(_logical_device, w_ctx->_depth_image_view_vec.back(), nullptr);
        w_ctx->_depth_image_view_vec.pop_back();
    }
    while (w_ctx->_depth_image_memory_vec.size())
    {
        vkFreeMemory(_logical_device, w_ctx->_depth_image_memory_vec.back(), nullptr);
        w_ctx->_depth_image_memory_vec.pop_back();
    }
    while (w_ctx->_depth_image_vec.size())
    {
        vkDestroyImage(_logical_device, w_ctx->_depth_image_vec.back(), nullptr);
        w_ctx->_depth_image_vec.pop_back();
    }
    while (w_ctx->_image_view_vec.size())
    {
        vkDestroyImageView(_logical_device, w_ctx->_image_view_vec.back(), nullptr);
        w_ctx->_image_view_vec.pop_back();
    }
    if (w_ctx->_swap_chain)
    {
        vkDestroySwapchainKHR(_logical_device, w_ctx->_swap_chain, nullptr);
        w_ctx->_swap_chain = nullptr;
    }
    return boole::True;
}

ref<window> vulkan_runtime::build_window(const window_desc& desc)
{
    AUTO_TRACE;

    _window_ctx_vec_lock.wait_write();
    escape_function ep =
        [=]() mutable
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
    escape_function ep =
        [=]() mutable
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
    AUTO_TRACE;

    auto r_shader = ref<vulkan_shader>::new_instance(desc, _self);
    if (!r_shader->load(desc._shader_path))
    {
        return ref<shader>();
    }
    return r_shader;
}

boole vulkan_runtime::register_dynamic_memory(const dynamic_memory_desc& desc)
{
    AUTO_TRACE;

    auto r_dm = ref<vulkan_dynamic_memory>::new_instance(_self);
    if (!r_dm->init(desc, r_dm))
    {
        return boole::False;
    }

    escape_function ef_uninit =
        [=]() mutable
        {
            r_dm->uninit();
        };

    if (!_dynamic_memory_map_lock.wait_write())
    {
        return boole::False;
    }

    escape_function ef_release_lock =
        [=]() mutable
        {
            _dynamic_memory_map_lock.write_release();
        };

    if (_dynamic_memory_map.find(desc._name) != _dynamic_memory_map.end())
    {
        // already has a same name dm
        return boole::False;
    }

    ef_uninit.disable();
    _dynamic_memory_map[desc._name] = r_dm;
    return boole::True;
}

boole vulkan_runtime::unregister_dynamic_memory(const string& dynamic_memory)
{
    AUTO_TRACE;

    auto r_dm = get_dynamic_memory(dynamic_memory);
    if (r_dm.empty())
    {
        return boole::False;
    }
    if (!r_dm->uninit())
    {
        return boole::False;
    }

    if (!_dynamic_memory_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _dynamic_memory_map_lock.write_release();
    };

    return _dynamic_memory_map.erase(dynamic_memory);
}

boole vulkan_runtime::update_dynamic_memory(const string& dynamic_memory, void* src)
{
    AUTO_TRACE;

    auto r_dm = get_dynamic_memory(dynamic_memory);
    if (r_dm.empty())
    {
        return boole::False;
    }
    return r_dm->update(src);
}

boole vulkan_runtime::setup_pipeline_dynamic_memory(const string& pipeline, const vector<string>& dm_vec)
{
    auto p = get_pipeline(pipeline);
    if (p.empty())
    {
        return boole::False;
    }

    return p->setup_dynamic_memory(dm_vec);
}

boole vulkan_runtime::register_texture_viewer(const texture_viewer_desc& desc)
{
    AUTO_TRACE;

    auto r_vv = ref<vulkan_texture_viewer>::new_instance(desc, _self);

    if (!_texture_viewer_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _texture_viewer_map_lock.write_release();
    };

    if (_texture_viewer_map.find(desc._name) != _texture_viewer_map.end())
    {
        // already has a same name pipeline
        return boole::False;
    }

    _texture_viewer_map[desc._name] = r_vv;
    return boole::True;
}

boole vulkan_runtime::unregister_texture_viewer(const string& texture_viewer)
{
    AUTO_TRACE;

    if (!_texture_viewer_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _texture_viewer_map_lock.write_release();
    };

    return _texture_viewer_map.erase(texture_viewer);
}

boole vulkan_runtime::load_texture_viewer(const string& texture_viewer)
{
    AUTO_TRACE;

    auto r_vv = get_texture_viewer(texture_viewer);
    if (r_vv.empty())
    {
        return boole::False;
    }
    return r_vv->load();
}

boole vulkan_runtime::unload_texture_viewer(const string& texture_viewer)
{
    AUTO_TRACE;

    auto r_vv = get_texture_viewer(texture_viewer);
    if (r_vv.empty())
    {
        return boole::False;
    }
    return r_vv->unload();
}

boole vulkan_runtime::update_pipeline_texture_viewer(const string& pipeline_name, const vector<string>& viewers)
{
    auto p = get_pipeline(pipeline_name);
    if (p.empty())
    {
        return boole::False;
    }

    return p->update_texture_viewer(viewers);
}

boole vulkan_runtime::register_vertices_viewer(const vertices_viewer_desc& desc)
{
    AUTO_TRACE;

    auto r_vv = ref<vulkan_vertices_viewer>::new_instance(desc, _self);

    if (!_vertices_viewer_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _vertices_viewer_map_lock.write_release();
    };

    if (_vertices_viewer_map.find(desc._name) != _vertices_viewer_map.end())
    {
        // already has a same name pipeline
        return boole::False;
    }

    _vertices_viewer_map[desc._name] = r_vv;
    return boole::True;
}

boole vulkan_runtime::unregister_vertices_viewer(const string& vertices_viewer)
{
    AUTO_TRACE;

    if (!_vertices_viewer_map_lock.wait_write())
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
    {
        _vertices_viewer_map_lock.write_release();
    };

    return _vertices_viewer_map.erase(vertices_viewer);
}

boole vulkan_runtime::load_vertices_viewer(const string& vertices_viewer)
{
    AUTO_TRACE;

    auto r_vv = get_vertices_viewer(vertices_viewer);
    if (r_vv.empty())
    {
        return boole::False;
    }
    return r_vv->load();
}

boole vulkan_runtime::unload_vertices_viewer(const string& vertices_viewer)
{
    AUTO_TRACE;

    auto r_vv = get_vertices_viewer(vertices_viewer);
    if (r_vv.empty())
    {
        return boole::False;
    }
    return r_vv->unload();
}

boole vulkan_runtime::setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers)
{
    auto p = get_pipeline(pipeline_name);
    if (p.empty())
    {
        return boole::False;
    }

    return p->setup_vertices(viewers);
}

boole vulkan_runtime::register_pipeline(const pipeline_desc& desc)
{
    AUTO_TRACE;

    auto w_ctx = get_window_context(desc._window_name);
    if (w_ctx.empty())
    {
        return boole::False;
    }

    auto r_pipeline = ref<vulkan_pipeline>::new_instance(_self.obs_of<vulkan_runtime>(), w_ctx);
    if (!r_pipeline->init(desc, r_pipeline.ref_of<pipeline>()))
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

boole vulkan_runtime::unregister_pipeline(const string& pipeline_name)
{
    AUTO_TRACE;

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

boole vulkan_runtime::load_pipeline_resource(const string& pipeline_name)
{
    AUTO_TRACE;

    auto p = get_pipeline(pipeline_name);
    if (p.empty())
    {
        return boole::False;
    }

    return p->load_resource();
}

boole vulkan_runtime::unload_pipeline_resource(const string& pipeline_name)
{
    AUTO_TRACE;

    auto p = get_pipeline(pipeline_name);
    if (p.empty())
    {
        return boole::False;
    }

    return p->unload_resource();
}

boole vulkan_runtime::render(const string& pipeline_name)
{
    AUTO_TRACE;

    auto p = get_pipeline(pipeline_name);
    if (p.empty())
    {
        return boole::False;
    }

    return p->render();
}

boole vulkan_runtime::wait_render_complete()
{
    AUTO_TRACE;

    if (vkDeviceWaitIdle(_logical_device) != VK_SUCCESS)
    {
        return boole::False;
    }
    return boole::True;
}

boole vulkan_runtime::setup_vk_buffer(
    VkDevice logical_device, VkPhysicalDevice device, sz_t size,
    boole single_queue_family, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    AUTO_TRACE;

    VkBuffer vk_buffer;
    VkDeviceMemory vk_memory;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode =
        //rt->_render_queue_family_idx == rt->_transfer_queue_family_idx ?
        single_queue_family ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;

    if (vkCreateBuffer(logical_device, &bufferInfo, nullptr, &vk_buffer) != VK_SUCCESS)
    {
        return boole::False;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logical_device, vk_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = get_vk_memory_type(device, memRequirements, properties);

    if (vkAllocateMemory(logical_device, &allocInfo, nullptr, &vk_memory) != VK_SUCCESS)
    {
        vkDestroyBuffer(logical_device, vk_buffer, nullptr);
        return boole::False;
    }

    if (vkBindBufferMemory(logical_device, vk_buffer, vk_memory, 0) != VK_SUCCESS)
    {
        vkDestroyBuffer(logical_device, vk_buffer, nullptr);
        vkFreeMemory(logical_device, vk_memory, nullptr);
        return boole::False;
    }

    buffer = vk_buffer;
    bufferMemory = vk_memory;
    return boole::True;
}

boole vulkan_runtime::clear_vk_buffer(VkDevice logical_device, VkBuffer buffer, VkDeviceMemory bufferMemory)
{
    vkDestroyBuffer(logical_device, buffer, nullptr);
    vkFreeMemory(logical_device, bufferMemory, nullptr);
    return boole::True;
}

boole vulkan_runtime::copy_vk_buffer(
    VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
    VkBuffer host_buf, s64 host_offset, VkBuffer device_buf, sz_t size)
{
    AUTO_TRACE;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transfer_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        return boole::False;
    }
    escape_function ef_release_command_buffer =
        [=]() mutable
    {
        vkFreeCommandBuffers(logical_device, transfer_command_pool, 1, &commandBuffer);
    };

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = host_offset;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    vkCmdCopyBuffer(commandBuffer, host_buf, device_buf, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);
    vkQueueSubmit(transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(transfer_queue);
    return boole::True;
}

boole vulkan_runtime::setup_vk_texture_image(
    VkDevice logical_device, VkPhysicalDevice physical_device,
    s64 width, s64 height, s64 mipmap_level,
    VkImage& image, VkDeviceMemory& imageMemory)
{
    AUTO_TRACE;

    boole rst = boole::False;
    VkImage vk_image;
    VkDeviceMemory vk_memory;

    escape_function ef_error_handle =
        [&]() mutable
        {
            if (vk_memory)
            {
                vkFreeMemory(logical_device, vk_memory, nullptr);
            }
            if (vk_image)
            {
                vkDestroyImage(logical_device, vk_image, nullptr);
            }
        };

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipmap_level;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | (mipmap_level > 1 ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0);
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(logical_device, &imageInfo, nullptr, &vk_image) != VK_SUCCESS)
    {
        return boole::False;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(logical_device, vk_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = get_vk_memory_type(physical_device, memRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(logical_device, &allocInfo, nullptr, &vk_memory) != VK_SUCCESS)
    {
        return boole::False;
    }

    if (vkBindImageMemory(logical_device, vk_image, vk_memory, 0) != VK_SUCCESS)
    {
        return boole::False;
    }

    ef_error_handle.disable();
    image = vk_image;
    imageMemory = vk_memory;
    return boole::True;
}

boole vulkan_runtime::clear_vk_image(VkDevice logical_device, VkImage image, VkDeviceMemory imageMemory)
{
    vkDestroyImage(logical_device, image, nullptr);
    vkFreeMemory(logical_device, imageMemory, nullptr);
    return boole::True;
}

boole vulkan_runtime::copy_vk_buffer_to_texture_image(
    VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
    VkBuffer host_buf, s64 host_offset, VkImage device_image, s64 width, s64 height)
{
    AUTO_TRACE;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transfer_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        return boole::False;
    }
    escape_function ef_release_command_buffer =
        [=]() mutable
        {
            vkFreeCommandBuffers(logical_device, transfer_command_pool, 1, &commandBuffer);
        };

    VkBufferImageCopy region = {};
    region.bufferOffset = host_offset;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { (u32)width, (u32)height, 1 };

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    vkCmdCopyBufferToImage(commandBuffer, host_buf, device_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    vkEndCommandBuffer(commandBuffer);
    vkQueueSubmit(transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(transfer_queue);
    return boole::True;
}

s64 vulkan_runtime::get_vk_memory_type(VkPhysicalDevice device, VkMemoryRequirements& requirements, VkFlags needed_properties)
{
    auto typeBits = requirements.memoryTypeBits;
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
    for (s64 i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if ((typeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & needed_properties) == needed_properties)
        {
            return i;
        }
    }
    return -1;
}

boole vulkan_runtime::convert_vk_image_layout(
    VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
    VkImage image, s64 convert_step, s64 mipmap_level, s64 mipmap_base_width, s64 mipmap_base_height)
{
    AUTO_TRACE;

    VkImageLayout old_layout;
    VkAccessFlagBits old_access;
    VkPipelineStageFlagBits old_stage_flag;

    VkImageLayout new_layout;
    VkAccessFlagBits new_access;
    VkPipelineStageFlagBits new_stage_flag;

    VkImageAspectFlags aspect_mask;
    s64 mipmap = 1;

    switch (convert_step)
    {
    case 1:
        old_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        old_access = VK_ACCESS_NONE_KHR;
        old_stage_flag = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        new_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        new_access = VK_ACCESS_TRANSFER_WRITE_BIT;
        new_stage_flag = VK_PIPELINE_STAGE_TRANSFER_BIT;
        aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
        mipmap = mipmap_level;
        break;

    case 2:
        if (mipmap_level > 1)
        {
            return convert_vk_image_mipmap_layout(
                logical_device, transfer_command_pool, transfer_queue, image,
                mipmap_level, mipmap_base_width, mipmap_base_height);
        }
        old_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        old_access = VK_ACCESS_TRANSFER_WRITE_BIT;
        old_stage_flag = VK_PIPELINE_STAGE_TRANSFER_BIT;
        new_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        new_access = VK_ACCESS_SHADER_READ_BIT;
        new_stage_flag = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
        break;

    case 3:
        old_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        old_access = VK_ACCESS_NONE_KHR;
        old_stage_flag = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        new_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        new_access = (VkAccessFlagBits)(VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
        new_stage_flag = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        break;

    case 4:
        old_layout = VK_IMAGE_LAYOUT_UNDEFINED;
        old_access = VK_ACCESS_NONE_KHR;
        old_stage_flag = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        new_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        new_access = (VkAccessFlagBits)(VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
        new_stage_flag = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
        break;

    default:
        return boole::False;
    }

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transfer_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        return boole::False;
    }
    escape_function ef_release_command_buffer =
        [=]() mutable
        {
            vkFreeCommandBuffers(logical_device, transfer_command_pool, 1, &commandBuffer);
        };

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspect_mask;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipmap;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = old_access;
    barrier.dstAccessMask = new_access;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    vkCmdPipelineBarrier(
        commandBuffer, old_stage_flag, new_stage_flag, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    vkEndCommandBuffer(commandBuffer);
    vkQueueSubmit(transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(transfer_queue);
    return boole::True;
}

boole vulkan_runtime::convert_vk_image_mipmap_layout(
    VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
    VkImage image, s64 mipmap_level, s64 mipmap_base_width, s64 mipmap_base_height)
{
    AUTO_TRACE;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transfer_command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(logical_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        return boole::False;
    }
    escape_function ef_release_command_buffer =
        [=]() mutable
        {
            vkFreeCommandBuffers(logical_device, transfer_command_pool, 1, &commandBuffer);
        };

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    for (s64 i = 1; i < mipmap_level; ++i)
    {
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(
            commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        s64 next_width = mipmap_base_width;
        s64 next_height = mipmap_base_height;
        if (mipmap_base_width >= mipmap_base_factor && mipmap_base_height >= mipmap_base_factor)
        {
            next_width = mipmap_base_width / mipmap_base_factor;
            next_height = mipmap_base_height / mipmap_base_factor;
        }

        VkImageBlit blit = {};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { (s32)mipmap_base_width, (s32)mipmap_base_height, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { (s32)next_width, (s32)next_height, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(
            commandBuffer,
            image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
            commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        mipmap_base_width = next_width;
        mipmap_base_height = next_height;
    }

    barrier.subresourceRange.baseMipLevel = mipmap_level - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(
        commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(transfer_queue);
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
