#pragma once

#include "Interface.hpp"

namespace gpx
{

struct pipeline_desc
{
    string                  _window_name;
    string                  _pipeline_name;
    vector<ref<shader>>     _shaders;
};

class pipeline
{
public:
    pipeline() = default;

    virtual ~pipeline() = default;

    virtual boole init(const pipeline_desc& desc) = 0;

    virtual boole uninit() = 0;
};

}
