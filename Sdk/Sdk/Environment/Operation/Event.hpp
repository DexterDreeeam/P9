#pragma once

class event final
{
    static const u64 _mem_sz = 16;

public:
    event();

    ~event();

    void wait();

    void set();

private:
    char _mem[_mem_sz];
};
