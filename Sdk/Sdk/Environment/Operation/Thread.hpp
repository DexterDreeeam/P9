#pragma once

class thread final
{
public:
    typedef void thread_call_back_function(void* par);

public:
    thread() :
        _ctx(nullptr)
    {
    }

    thread(const thread& rhs) :
        _ctx(rhs._ctx)
    {
    }

    thread& operator =(const thread& rhs)
    {
        _ctx = rhs._ctx;
        return *this;
    }

    ~thread() = default;

public:
    boole init(thread_call_back_function* fn);

    boole uninit();

    boole start(void* par);

    boole wait();

    boole force_stop();

public:
    static u64 id();

private:
    void* _ctx;
};
