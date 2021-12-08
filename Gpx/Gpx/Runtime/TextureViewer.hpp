#pragma once

#include "../../../Sdk/Interface.hpp"

namespace gpx
{

struct texture_viewer_desc
{
    string  _name;
    string  _file_path;
    boole   _mipmap_enable;
};

enum class texture_viewer_state : s64
{
    Offline = 0x20,
    Transition,
    Online,
};

class runtime;

class texture_viewer
{
public:
    texture_viewer();

    virtual ~texture_viewer() = default;

    virtual boole load() = 0;

    virtual boole unload() = 0;

public:
    texture_viewer_state state() const;

protected:
    boole transfer_state(texture_viewer_state from, texture_viewer_state to);

protected:
    atom<s64>      _state;
};

}
