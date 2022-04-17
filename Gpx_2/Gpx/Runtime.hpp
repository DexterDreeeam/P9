#pragma once

#include "../../Sdk/_Interface.hpp"
#include "../_Definition.hpp"

namespace gpx
{

struct runtime_desc
{
    graphics_api_type   type;
    string              preferred_device_name;
    s64                 frame_count;
    s64                 msaa_level; // 0 ~ 6, sample count 2^0 ~ 2^6
    boole               debug_mode;
};

class runtime
{
public:
    runtime() = default;

    runtime(const runtime&) = delete;

    runtime& operator =(const runtime&) = delete;

    virtual ~runtime() = default;

    virtual const char* obj_type() const override { return "gpx::runtime"; }

    virtual u128 obj_type_id() const override { return u128(0xae258bd036ad0358, 0xf36adf2479c0358b); }

public:
    // device

    virtual vector<string> list_device() = 0;

    // window

    virtual boole build_window(const window_desc& desc) = 0;

    virtual boole destroy_window(const string& window_name) = 0;

    virtual ref<window> get_window(const string& window_name) = 0;

    // shader

    virtual boole build_shader(const shader_desc& desc) = 0;

    virtual boole destroy_shader(const string& shader_name) = 0;

    // dynamic memory

    virtual boole build_dynamic_memory(const dynamic_memory_desc& desc) = 0;

    virtual boole destroy_dynamic_memory(const string& dm_name) = 0;

    virtual boole update_dynamic_memory(const string& dm_name, void* src) = 0;

    virtual boole setup_pipeline_dynamic_memory(const string& pipeline, const vector<string>& dm_name_vec) = 0;

    // vertices

    virtual boole build_vertices_viewer(const vertices_viewer_desc& desc) = 0;

    virtual boole destroy_vertices_viewer(const string& vv_name) = 0;

    virtual boole load_vertices_viewer(const string& vv_name) = 0;

    virtual boole unload_vertices_viewer(const string& vv_name) = 0;

    virtual boole setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers) = 0;

    // texture

    virtual boole build_texture_viewer(const texture_viewer_desc& desc) = 0;

    virtual boole destroy_texture_viewer(const string& tv_name) = 0;

    virtual boole load_texture_viewer(const string& tv_name) = 0;

    virtual boole unload_texture_viewer(const string& tv_name) = 0;

    virtual boole update_pipeline_texture_viewer(const string& pipeline_name, const vector<string>& viewers) = 0;

    // pipeline

    virtual boole build_pipeline(const pipeline_desc& desc) = 0;

    virtual boole destroy_pipeline(const string& pipeline_name) = 0;

    virtual boole load_pipeline_resource(const string& pipeline_name) = 0;

    virtual boole unload_pipeline_resource(const string& pipeline_name) = 0;

    virtual boole render(const string& pipeline_name) = 0;

    virtual boole wait_render_complete() = 0;
};

}
