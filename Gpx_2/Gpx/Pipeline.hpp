#pragma once

#include "../../ResourceMgr/_Interface.hpp"
#include "_Interface.hpp"

namespace gpx
{

enum class pipeline_render_type : s64
{
    basic,
    // polygon
    // frame
};

struct pipeline_desc
{
    string                  window_name;
    string                  pipeline_name;
    pipeline_render_type    render_type;
    vertex_type             vertex_type;
    vector<ref<shader>>     shaders;
    s64                     dynamic_memory_count;
    s64                     texture_viewer_count;
};

/*
 * Uninit -> InitingOrUniniting -> Inited
 *                                 Inited -> SetupInProgress
 *                                 Inited <- SetupInProgress
 *                                 Inited ---------------------> ResourceLoadingOrUnloading -> ResourceLoaded -> CommandSubmitting
 *                                                                                             ResourceLoaded <- CommandSubmitting
 *                                                                                             ResourceLoaded -> CommandSubmitting
 *                                 Inited <--------------------- ResourceLoadingOrUnloading <- ResourceLoaded <- CommandSubmitting
 * Uninit <- InitingOrUniniting <- Inited
 */

enum class pipeline_state : s64
{
    Uninit,
    InitingOrUniniting,
    Inited,
    SetupInProgress,
    ResourceLoadingOrUnloading,
    ResourceLoaded,
    CommandSubmitting,
};

class pipeline
{
public:
    pipeline();

    virtual ~pipeline() = default;

    virtual boole init(const pipeline_desc& desc, obs<pipeline> self) = 0;

    virtual boole uninit() = 0;

    virtual boole setup_dynamic_memory(const vector<string>& dm_vec) = 0;

    virtual boole update_texture_viewer(const vector<string>& tv_vec) = 0;

    virtual boole setup_vertices(const vector<string>& vertices_viewer_vec) = 0;

    virtual boole load_resource() = 0;

    virtual boole unload_resource() = 0;

    virtual boole render() = 0;

protected:
    pipeline_state state() const;

    boole transfer_state(pipeline_state from, pipeline_state to);

private:
    atom<s64>   _state;
};

}
