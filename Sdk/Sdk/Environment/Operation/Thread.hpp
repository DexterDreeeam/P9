#pragma once

class thread final
{
    static const s64 _mem_sz = sizeof(ref<int>);

public:
    template<typename Fn_Ty, typename ...Args>
    static thread create(Fn_Ty fn, Args... args);

public:


private:
    char _mem[_mem_sz];
};
