#pragma once

namespace gpx
{

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

}
