#pragma once

#include "Interface.hpp"

namespace gpx
{

enum class shader_type : s64
{
    VERTEX,
    FRAGMENT,
    TESSELLATION,
    GEOMETRY,
};

struct shader_desc
{
    shader_type  _type;
    string       _window_name;
    string       _shader_name;
    string       _shader_path;
};

class shader
{
public:
    shader() = default;

    virtual ~shader() = default;

    virtual boole load(const string& shader_path) = 0;
};

}
