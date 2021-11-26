#pragma once

namespace gpx
{

enum class vertex_type : s64
{
    None = 0x10,
    POS_COLOR,
};

class vertex_none
{
public:
    constexpr static vertex_type type()
    {
        return vertex_type::None;
    }
};

class vertex_pos_color
{
public:
    constexpr static vertex_type type()
    {
        return vertex_type::POS_COLOR;
    }

public:
    vec3 _pos;
    vec3 _color;
};

class vertex_index
{
public:
    u32 _idx;
};

}
