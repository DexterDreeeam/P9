#pragma once

#include "Pipeline.hpp"
#include "Shader.hpp"
#include "VerticesViewer.hpp"

namespace gpx
{

struct runtime_desc
{
    enum class _type : s64
    {
        None = 0,

        Vulkan,

        DirectX,
    };

    _type   type;
    boole   debug_mode;
};

class runtime
{
public:
    runtime() = default;

    runtime(const runtime&) = delete;

    runtime& operator =(const runtime&) = delete;

    virtual ~runtime() = default;

public:
    virtual void setup_self(obs<runtime> obs_rt) = 0;

public:
    static ref<runtime> build(const runtime_desc& desc);

public:
    // runtime

    virtual boole init(const string& preferred_device_name = "") = 0;

    virtual boole uninit() = 0;

    virtual vector<string> list_device() = 0;

    // window

    virtual ref<window> build_window(const window_desc& desc) = 0;

    virtual boole remove_window(const string& window_name) = 0;

    virtual ref<window> get_window(const string& window_name) = 0;

    // shader

    virtual ref<shader> build_shader(const shader_desc& desc) = 0;

    // vertices

    virtual boole register_vertices_viewer(const vertices_viewer_desc& desc) = 0;

    virtual boole unregister_vertices_viewer(const string& vertices_viewer) = 0;

    virtual boole load_vertices_viewer(const string& vertices_viewer) = 0;

    virtual boole unload_vertices_viewer(const string& vertices_viewer) = 0;

    // pipeline

    virtual boole register_pipeline(const pipeline_desc& desc) = 0;

    virtual boole unregister_pipeline(const string& pipeline_name) = 0;

    virtual boole setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers) = 0;

    virtual boole load_pipeline_resource(const string& pipeline_name) = 0;

    virtual boole unload_pipeline_resource(const string& pipeline_name) = 0;

    virtual boole render(const string& pipeline_name) = 0;

    virtual boole wait_render_complete(const string& window_name) = 0;
};

}
