#pragma once

#include "Interface.hpp"

namespace gpx
{

class vulkan_runtime : public runtime
{
public:
    vulkan_runtime(const runtime_desc& desc);

    virtual ~vulkan_runtime() override;

public:
    virtual void setup_self(obs<runtime> obs_rt) override;

public:
    virtual boole init() override;

    virtual boole uninit() override;

    virtual vector<string> list_device() override;

    virtual boole select_device(const string& device_name = "") override;

    virtual ref<window> build_window(const window_desc& desc) override;

    virtual ref<window> get_window(const string& window_name) override;

private:
    boole build_device_resource(VkPhysicalDevice physical_device);

    boole clear_device_resource();

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

private:
    runtime_desc          _desc;
    obs<runtime>          _self;

    // vulkan core
    vector<const char*>   _vec_validation_layer;
    atom<VkInstance*>     _instance;

    // device resource
    VkPhysicalDevice      _physical_device;
    VkDevice              _logical_device;
    VkQueue               _render_queue;
    VkQueue               _transfer_queue;

    // window resource
    vector<ref<window>>   _vec_window;
    rw_lock               _vec_window_lock;
};

}
