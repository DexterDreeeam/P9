#pragma once

#include "../../../Sdk/Interface.hpp"

namespace gpx
{

struct runtime_desc
{
    enum class _type : s64
    {
        Default = 0,

        Vulkan = Default,

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
    static ref<runtime> build(const runtime_desc& desc);

public:
    virtual boole init() = 0;

    virtual boole uninit() = 0;
};

}
