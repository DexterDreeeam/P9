#pragma once

class event final
{
public:
    event() :
        _ctx(nullptr)
    {}

    event(const event& rhs) :
        _ctx(rhs._ctx)
    {}

    event& operator =(const event& rhs)
    {
        _ctx = rhs._ctx;
        return *this;
    }

    ~event() = default;

    boole init();

    boole is_init() { return _ctx != nullptr; }

    boole uninit();

    boole wait();

    boole set();

private:
    void* _ctx;
};
