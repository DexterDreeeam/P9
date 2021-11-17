#pragma once

#include "Interface.hpp"

namespace gpx
{

struct window_context
{
    ref<window>              _window;
    string                   _preferred_device_name;

    VkPhysicalDevice         _physical_device;
    VkDevice                 _logical_device;
    VkQueue                  _render_queue;
    VkQueue                  _present_queue;
    VkQueue                  _transfer_queue;
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

private:
    VkInstance get_vk_instance() { return *_instance.get(); }

    ref<window_context> get_window_context(const string& window_name);

    boole window_start_callback(const string& window_name);

    boole window_stop_callback(const string& window_name);

    boole build_device_resource(VkPhysicalDevice physical_device, ref<window_context> w_ctx);

    boole clear_device_resource(ref<window_context> w_ctx);

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
    vector<ref<window_context>>   _vec_window_ctx;
    rw_lock                       _vec_window_ctx_lock;
};

}
