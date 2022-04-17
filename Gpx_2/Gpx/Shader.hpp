#pragma once

#include "../../Sdk/_Interface.hpp"

namespace gpx
{

enum class shader_type : s64
{
    VERTEX,
    TESSELLATION,
    GEOMETRY,
    FRAGMENT,
};

struct shader_desc
{
    shader_type  type;
    string       shader_name;
    string       shader_path;
};

class shader
{
public:
    shader() = default;

    virtual ~shader() = default;

    virtual boole load(const string& shader_path) = 0;

    virtual boole unload() = 0;

    virtual shader_type type() = 0;
};

}
