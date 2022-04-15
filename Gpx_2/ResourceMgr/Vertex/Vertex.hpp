#pragma once

namespace gpx
{

enum class vertex_type : s64
{
    None = 0x10,
    POS_COLOR,
    POS_TEXTURE,
};

class vertex_none
{
public:
    constexpr static vertex_type type()
    {
        return vertex_type::None;
    }
};

class vertex_index
{
public:
    u32 _idx;
};

}
