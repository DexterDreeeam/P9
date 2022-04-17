#pragma once

#include "../../Sdk/_Interface.hpp"

namespace gpx
{

struct vertices_viewer_desc
{
    string  _name;
    string  _file_path;
};

enum class vertices_viewer_state : s64
{
    Offline = 0x20,
    Transition,
    Online,
};

class runtime;

class vertices_viewer
{
public:
    vertices_viewer();

    virtual ~vertices_viewer() = default;

    virtual boole load() = 0;

    virtual boole unload() = 0;

public:
    vertices_viewer_state state() const;

protected:
    boole transfer_state(vertices_viewer_state from, vertices_viewer_state to);

protected:
    atom<s64>      _state;
};

}
