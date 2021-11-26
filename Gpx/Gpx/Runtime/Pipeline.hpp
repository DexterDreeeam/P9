#pragma once

#include "Interface.hpp"

namespace gpx
{

struct pipeline_desc
{
    string                  _window_name;
    string                  _pipeline_name;
    vertex_type             _vertex_type;
    vector<ref<shader>>     _shaders;
};

class pipeline
{
public:
    pipeline() = default;

    virtual ~pipeline() = default;

    virtual boole init(const pipeline_desc& desc) = 0;

    virtual boole uninit() = 0;

    virtual boole setup_vertices_buffer(ref<vertices_buffer> vertices_buffer) = 0;

    virtual boole clear_vertices_buffer() = 0;

    virtual boole load_resource() = 0;

    virtual boole unload_resource() = 0;

    virtual boole render() = 0;
};

}
