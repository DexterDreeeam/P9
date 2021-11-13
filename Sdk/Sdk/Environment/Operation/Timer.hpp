#pragma once

class timer final
{
public:
    typedef void timer_call_back_function(void* par);

public:
    timer();

    timer(const timer& rhs);

    timer& operator =(const timer& rhs);

    ~timer() = default;

public:
    boole init(timer_call_back_function* fn);

    boole is_init();

    boole uninit();

    boole trigger(void* par, u64 ms);

private:
    void* _ctx;
};
