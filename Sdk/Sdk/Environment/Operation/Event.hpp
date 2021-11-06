#pragma once

class event final
{
public:
    event();

    ~event();

    void wait();

    void set();

private:
    char _mem[16];
};
