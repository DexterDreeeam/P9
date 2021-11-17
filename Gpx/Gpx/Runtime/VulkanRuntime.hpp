#pragma once

#include "Interface.hpp"
#include "VulkanPipeline.hpp"

namespace gpx
{

struct vulkan_window_context
{
    ref<window>                _window;
    string                     _preferred_device_name;

    VkPhysicalDevice           _physical_device;
    VkDevice                   _logical_device;
    u64                        _render_queue_family_idx;
    u64                        _present_queue_family_idx;
    u64                        _transfer_queue_family_idx;
    VkQueue                    _render_queue;
    VkQueue                    _present_queue;
    VkQueue                    _transfer_queue;

    VkSurfaceFormatKHR         _surface_format;
    VkPresentModeKHR           _present_mode;
    VkExtent2D                 _swap_chain_extent;
    VkSwapchainKHR             _swap_chain;
    vector<VkImage>            _swap_chain_image_vec;
    vector<VkImageView>        _image_view_vec;

    vector<ref<pipeline>>      _pipelines;
};

class vulkan_runtime : public runtime
{
    friend class glfw_window;

public:
    vulkan_runtime(const runtime_desc& desc);

    virtual ~vulkan_runtime() override;

public:
    virtual void setup_self(obs<runtime> obs_rt) override;

public:
    virtual boole init() override;

    virtual boole uninit() override;

    virtual vector<string> list_device() override;

    virtual ref<window> build_window(const window_desc& desc, const string& preferred_device_name = "") override;

    virtual boole remove_window(const string& window_name) override;

    virtual ref<window> get_window(const string& window_name) override;

    virtual boole register_pipeline() override;

    virtual boole unregister_pipeline() override;

private:
    VkInstance get_vk_instance() { return *_instance.get(); }

    ref<vulkan_window_context> get_window_context(const string& window_name);

    boole window_start_callback(const string& window_name);

    boole window_stop_callback(const string& window_name);

    boole build_device_hardware_resource(VkPhysicalDevice physical_device, ref<vulkan_window_context> w_ctx);

    boole build_device_image_resource(ref<vulkan_window_context> w_ctx);

    boole clear_device_resource(ref<vulkan_window_context> w_ctx);

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

    // window resource
    vector<
        ref<vulkan_window_context>
    >                             _window_ctx_vec;
    rw_lock                       _window_ctx_vec_lock;
};

}
