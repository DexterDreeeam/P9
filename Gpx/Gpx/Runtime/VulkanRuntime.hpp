#pragma once

#include "Interface.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVerticesViewer.hpp"

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

public:
    vulkan_runtime(const runtime_desc& desc);

    virtual ~vulkan_runtime() override;

public:
    virtual void setup_self(obs<runtime> obs_rt) override;

public:
    virtual boole init(const string& preferred_device_name = "") override;

    virtual boole uninit() override;

    virtual vector<string> list_device() override;

    virtual ref<window> build_window(const window_desc& desc) override;

    virtual boole remove_window(const string& window_name) override;

    virtual ref<window> get_window(const string& window_name) override;

    virtual ref<shader> build_shader(const shader_desc& desc) override;

    virtual boole register_pipeline(const pipeline_desc& desc) override;

    virtual boole unregister_pipeline(const string& pipeline_name) override;

    virtual boole setup_vertices_buffer(const string& pipeline_name, ref<vertices_buffer> buffer) override;

    virtual boole clear_vertices_buffer(const string& pipeline_name) override;

    virtual boole load_pipeline_resource(const string& pipeline_name) override;

    virtual boole unload_pipeline_resource(const string& pipeline_name) override;

    virtual boole render(const string& pipeline_name) override;

    virtual boole wait_render_complete(const string& window_name) override;

public:
    VkInstance get_vk_instance() { return *_instance.get(); }

    VkDevice get_vk_logical_device() { return _logical_device; }

    ref<vulkan_window_context> get_window_context(const string& window_name);

    ref<pipeline> get_pipeline(const string& pipeline_name);

    boole window_start_callback(const string& window_name);

    boole window_stop_callback(const string& window_name);

    boole build_device_hardware_resource(VkPhysicalDevice physical_device, ref<vulkan_window_context> w_ctx);

    boole build_device_image_resource(ref<vulkan_window_context> w_ctx);

    boole clear_device_hardware_resource();

    boole clear_device_image_resource(ref<vulkan_window_context> w_ctx);

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

private:
    runtime_desc                  _desc;
    obs<runtime>                  _self;

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
};

}
