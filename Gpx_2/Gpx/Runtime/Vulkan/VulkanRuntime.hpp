#pragma once

namespace gpx
{

struct vulkan_window_context
{
    ref<window>                _window;

    VkSurfaceFormatKHR         _surface_format;
    VkPresentModeKHR           _present_mode;
    VkExtent2D                 _swap_chain_extent;
    VkSwapchainKHR             _swap_chain;
    vector<VkImage>            _swap_chain_image_vec;
    vector<VkImageView>        _image_view_vec;

    vector<VkImage>            _msaa_image_vec;
    vector<VkDeviceMemory>     _msaa_image_memory_vec;
    vector<VkImageView>        _msaa_image_view_vec;

    VkFormat                   _depth_format;
    vector<VkImage>            _depth_image_vec;
    vector<VkDeviceMemory>     _depth_image_memory_vec;
    vector<VkImageView>        _depth_image_view_vec;

    vector<VkSemaphore>        _image_available_sema_vec;
    vector<VkSemaphore>        _render_complete_sema_vec;
    vector<VkFence>            _inflight_fence_vec;
    vector<VkFence>            _reordered_fence_vec;
    s64                        _fence_counter;
};

class vulkan_runtime : public runtime
{
    friend class glfw_window;
    friend class vulkan_pipeline;
    friend class vulkan_vertices_viewer;
    friend class vulkan_texture_viewer;
    friend class vulkan_dynamic_memory;

public:
    // actually, only 2 is valid for vulkan
    static const s64 mipmap_base_factor = 2;

public:
    vulkan_runtime(const runtime_desc& desc);

    virtual ~vulkan_runtime() override;

public:
    virtual void setup_self(obs<runtime> obs_rt) override;

public:
    virtual boole init(const string& preferred_device_name = "") override;

    virtual boole uninit() override;

    virtual vector<string> list_device() override;

    // window

    virtual ref<window> build_window(const window_desc& desc) override;

    virtual boole remove_window(const string& window_name) override;

    virtual ref<window> get_window(const string& window_name) override;

    // shader

    virtual ref<shader> build_shader(const shader_desc& desc) override;

    // dynamic memory

    virtual boole register_dynamic_memory(const dynamic_memory_desc& desc) override;

    virtual boole unregister_dynamic_memory(const string& dynamic_memory) override;

    virtual boole update_dynamic_memory(const string& dynamic_memory, void* src) override;

    virtual boole setup_pipeline_dynamic_memory(const string& pipeline, const vector<string>& dm_vec) override;

    // vertices viewer

    virtual boole register_vertices_viewer(const vertices_viewer_desc& desc) override;

    virtual boole unregister_vertices_viewer(const string& vertices_viewer) override;

    virtual boole load_vertices_viewer(const string& vertices_viewer) override;

    virtual boole unload_vertices_viewer(const string& vertices_viewer) override;

    virtual boole setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers) override;

    // texture

    virtual boole register_texture_viewer(const texture_viewer_desc& texture_viewer) override;

    virtual boole unregister_texture_viewer(const string& texture_viewer) override;

    virtual boole load_texture_viewer(const string& texture_viewer) override;

    virtual boole unload_texture_viewer(const string& texture_viewer) override;

    virtual boole update_pipeline_texture_viewer(const string& pipeline_name, const vector<string>& viewers) override;

    // pipeline

    virtual boole register_pipeline(const pipeline_desc& desc) override;

    virtual boole unregister_pipeline(const string& pipeline_name) override;

    virtual boole load_pipeline_resource(const string& pipeline_name) override;

    virtual boole unload_pipeline_resource(const string& pipeline_name) override;

    virtual boole render(const string& pipeline_name) override;

    virtual boole wait_render_complete() override;

public:
    VkInstance get_vk_instance() { return *_instance.get(); }

    VkDevice get_vk_logical_device() { return _logical_device; }

    VkPhysicalDevice get_vk_physical_device() { return _physical_device; }

    ref<vulkan_window_context> get_window_context(const string& window_name);

    ref<vulkan_vertices_viewer> get_vertices_viewer(const string& vertices_viewer);

    ref<vulkan_texture_viewer> get_texture_viewer(const string& texture_viewer);

    ref<vulkan_dynamic_memory> get_dynamic_memory(const string& dynamic_memory);

    ref<vulkan_pipeline> get_pipeline(const string& pipeline_name);

    boole window_start_callback(const string& window_name);

    boole window_stop_callback(const string& window_name);

    boole build_device_hardware_resource(VkPhysicalDevice physical_device, ref<vulkan_window_context> w_ctx);

    boole build_device_image_resource(ref<vulkan_window_context> w_ctx);

    boole clear_device_hardware_resource();

    boole clear_device_image_resource(ref<vulkan_window_context> w_ctx);

public:
    static boole setup_vk_buffer(
        VkDevice logical_device, VkPhysicalDevice device, sz_t size,
        boole single_queue_family, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    static boole clear_vk_buffer(VkDevice logical_device, VkBuffer buffer, VkDeviceMemory bufferMemory);

    static boole copy_vk_buffer(
        VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
        VkBuffer host_buf, s64 host_offset, VkBuffer device_buf, sz_t size);

    static boole setup_vk_texture_image(
        VkDevice logical_device, VkPhysicalDevice physical_device,
        s64 width, s64 height, s64 mipmap_level,
        VkImage& image, VkDeviceMemory& imageMemory);

    static boole clear_vk_image(VkDevice logical_device, VkImage image, VkDeviceMemory imageMemory);

    static boole copy_vk_buffer_to_texture_image(
        VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
        VkBuffer host_buf, s64 host_offset, VkImage device_image, s64 width, s64 height);

    static s64 get_vk_memory_type(VkPhysicalDevice device, VkMemoryRequirements& requirements, VkFlags needed_properties);

private:
    // convert step:
    //
    // 1. Texture get ready for transfer
    //        VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    //
    // 2. Texture get ready for sampling
    //        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    //
    // 3. Depth get ready for Z-Buffer recording
    //        VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    //
    // mipmap_level
    //     Valid only if convert_step is 1 or 2
    static boole convert_vk_image_layout(
        VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
        VkImage image, s64 convert_step, s64 mipmap_level = 1, s64 mipmap_base_width = 0, s64 mipmap_base_height = 0);

    static boole convert_vk_image_mipmap_layout(
        VkDevice logical_device, VkCommandPool transfer_command_pool, VkQueue transfer_queue,
        VkImage image, s64 mipmap_level, s64 mipmap_base_width, s64 mipmap_base_height);

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

private:
    runtime_desc                  _desc;
    obs<vulkan_runtime>           _self;

    // vulkan core
    vector<const char*>           _vec_validation_layer;
    static atom<VkInstance*>      _instance;
    lock                          _resource_lk;
    string                        _preferred_device_name;
    VkPhysicalDevice              _physical_device;
    VkDevice                      _logical_device;
    VkSampleCountFlagBits         _msaa;
    u64                           _render_queue_family_idx;
    u64                           _present_queue_family_idx;
    u64                           _transfer_queue_family_idx;
    VkQueue                       _render_queue;
    VkQueue                       _present_queue;
    VkQueue                       _transfer_queue;
    VkCommandPool                 _render_command_pool;
    VkCommandPool                 _transfer_command_pool;

    // window resource
    vector<
        ref<vulkan_window_context>
    >                             _window_ctx_vec;
    rw_lock                       _window_ctx_vec_lock;

    map<
        string, ref<vulkan_pipeline>
    >                             _pipeline_map;
    rw_lock                       _pipeline_map_lock;

    map<
        string, ref<vulkan_vertices_viewer>
    >                             _vertices_viewer_map;
    rw_lock                       _vertices_viewer_map_lock;

    map<
        string, ref<vulkan_texture_viewer>
    >                             _texture_viewer_map;
    rw_lock                       _texture_viewer_map_lock;

    map<
        string, ref<vulkan_dynamic_memory>
    >                             _dynamic_memory_map;
    rw_lock                       _dynamic_memory_map_lock;
};

}
