#pragma once

class thread final
{
public:
    typedef void thread_call_back_function(void* par);

public:
    thread();

    thread(const thread& rhs);

    thread(thread&& rhs) noexcept;

    thread& operator =(const thread& rhs);

    thread& operator =(thread&& rhs) noexcept;

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
