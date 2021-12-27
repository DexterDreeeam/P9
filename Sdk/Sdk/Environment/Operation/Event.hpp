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

    event(event&& rhs) :
        _ctx(rhs._ctx)
    {
        rhs._ctx = nullptr;
    }

    event& operator =(const event& rhs)
    {
        _ctx = rhs._ctx;
        return *this;
    }

    event& operator =(event&& rhs)
    {
        _ctx = rhs._ctx;
        rhs._ctx = nullptr;
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
