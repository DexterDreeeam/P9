#pragma once

#include "../../../Sdk/Interface.hpp"

namespace gpx
{

struct window_desc
{
    enum class _type : s64
    {
        Default = 0,

        Glfw = Default,

        P9,
    };

    window_desc() :
        name(), width(800), height(600), type(_type::Default)
    {
    }

    _type    type;
    string   name;
    s64      width;
    s64      height;
};

class window
{
public:
    window(const window&) = delete;

    window& operator =(const window&) = delete;

    virtual ~window() = default;

protected:
    window() = default;

public:
    static ref<window> build(const window_desc& desc);

public:
    virtual boole start() = 0;

    virtual boole stop() = 0;

    virtual boole present(s64 my_image) = 0;

    virtual boole poll_event() = 0;

    virtual boole is_running() = 0;
};

}
