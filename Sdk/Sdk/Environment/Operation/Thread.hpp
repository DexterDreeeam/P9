#pragma once

class thread final
{
public:
    typedef void thread_call_back_function(void* par);

public:
    thread();

    thread(const thread& rhs);

    thread& operator =(const thread& rhs);

    ~thread() = default;

public:
    boole init(thread_call_back_function* fn);

    boole is_init();

    boole uninit();

    boole start(void* par);

    boole wait();

    boole force_stop();

private:
    void* _ctx;
};


_INLINE_ thread::thread() :
    _ctx(nullptr)
{
}

_INLINE_ thread::thread(const thread& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ thread& thread::operator =(const thread& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ boole thread::is_init()
{
    return _ctx != nullptr;
}
