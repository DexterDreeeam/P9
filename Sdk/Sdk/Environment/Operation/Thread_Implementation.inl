#pragma once

_INLINE_ thread::thread() :
    _ctx(nullptr)
{
}

_INLINE_ thread::thread(const thread& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ thread::thread(thread&& rhs) :
    _ctx(rhs._ctx)
{
    rhs._ctx = nullptr;
}

_INLINE_ thread& thread::operator =(const thread& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ thread& thread::operator =(thread&& rhs)
{
    _ctx = rhs._ctx;
    rhs._ctx = nullptr;
    return *this;
}

_INLINE_ boole thread::is_init()
{
    return _ctx != nullptr;
}
