#pragma once

namespace gpx
{

class vertex_pos_texture
{
public:
    constexpr static vertex_type type()
    {
        return vertex_type::POS_TEXTURE;
    }

public:
    vec3 _pos;
    vec2 _texture;
};

}
