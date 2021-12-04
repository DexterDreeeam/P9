#pragma once

namespace gpx
{

struct window_desc
{
    window_desc() :
        name(), width(800), height(600)
    {
    }

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
    virtual string name() = 0;

    virtual boole start() = 0;

    virtual boole stop() = 0;

    virtual boole present(s64 my_image) = 0;

    virtual boole poll_event() = 0;

    virtual boole is_running() = 0;
};

}
