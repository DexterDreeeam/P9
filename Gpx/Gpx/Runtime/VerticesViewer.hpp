#pragma once

#include "Interface.hpp"

namespace gpx
{

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
    vertices_viewer() = default;

    virtual ~vertices_viewer() = default;

    virtual boole init(const string& path, obs<runtime> rt) = 0;

    virtual boole uninit() = 0;

    virtual boole load() = 0;

    virtual boole unload() = 0;

private:
    string         _name;
    string         _file_path;
    atom<s64>      _state;
};

}
