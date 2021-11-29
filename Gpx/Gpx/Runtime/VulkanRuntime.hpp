#pragma once

#include "Interface.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVerticesViewer.hpp"
#include "VulkanDynamicMemory.hpp"

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
    friend class vulkan_dynamic_memory;

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

    // vertices viewer

    virtual boole register_vertices_viewer(const vertices_viewer_desc& desc) override;

    virtual boole unregister_vertices_viewer(const string& vertices_viewer) override;

    virtual boole load_vertices_viewer(const string& vertices_viewer) override;

    virtual boole unload_vertices_viewer(const string& vertices_viewer) override;

    // pipeline

    virtual boole register_pipeline(const pipeline_desc& desc) override;

    virtual boole unregister_pipeline(const string& pipeline_name) override;

    virtual boole setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers) override;

    virtual boole load_pipeline_resource(const string& pipeline_name) override;

    virtual boole unload_pipeline_resource(const string& pipeline_name) override;

    virtual boole render(const string& pipeline_name) override;

    virtual boole wait_render_complete(const string& window_name) override;

public:
    VkInstance get_vk_instance() { return *_instance.get(); }

    VkDevice get_vk_logical_device() { return _logical_device; }

    VkPhysicalDevice get_vk_physical_device() { return _physical_device; }

    ref<vulkan_window_context> get_window_context(const string& window_name);

    ref<vulkan_vertices_viewer> get_vertices_viewer(const string& vertices_viewer);

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

    static s64 get_vk_memory_type(VkPhysicalDevice device, VkMemoryRequirements& requirements, VkFlags needed_properties);


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
        string, ref<vulkan_dynamic_memory>
    >                             _dynamic_memory_map;
    rw_lock                       _dynamic_memory_map_lock;
};

}
