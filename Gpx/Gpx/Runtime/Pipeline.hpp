#pragma once

#include "../../ResourceMgr/Interface.hpp"
#include "Shader.hpp"

namespace gpx
{

struct pipeline_desc
{
    string                  _window_name;
    string                  _pipeline_name;
    vertex_type             _vertex_type;
    vector<ref<shader>>     _shaders;
    vector<string>          _dynamic_memories;
};

/*
 * Uninit -> InitingOrUniniting -> Inited
 *                                 Inited -> SetupInProgress
 *                                 Inited <- SetupInProgress
 *                                 Inited ---------------------> ResourceLoadingOrUnloading -> ResourceLoaded
 *                                                                                             ResourceLoaded -> CommandSubmitting
 *                                                                                             ResourceLoaded <- CommandSubmitting
 *                                 Inited <--------------------- ResourceLoadingOrUnloading <- ResourceLoaded
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
