#pragma once

#include "Interface.hpp"

namespace gpx
{

struct runtime_desc
{
    enum class _type : s64
    {
        None = 0,

        Vulkan,

        DirectX,
    };

    _type   type;
    boole   debug_mode;
};

class runtime
{
public:
    runtime() = default;

    runtime(const runtime&) = delete;

    runtime& operator =(const runtime&) = delete;

    virtual ~runtime() = default;

public:
    virtual void setup_self(obs<runtime> obs_rt) = 0;

public:
    static ref<runtime> build(const runtime_desc& desc);

public:
    virtual boole init() = 0;

    virtual boole uninit() = 0;

    virtual ref<window> build_window(const window_desc& desc) = 0;

    virtual ref<window> get_window(const string& window_name) = 0;
};

}
