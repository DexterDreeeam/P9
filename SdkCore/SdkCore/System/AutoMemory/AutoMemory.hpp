#pragma once

class auto_memory
{
public:
    auto_memory(sz_t sz) :
        _mem(memory::alloc(sz))
    {
    }

    auto_memory(auto_memory&& rhs) noexcept :
        _mem(rhs._mem)
    {
        rhs._mem = nullptr;
    }

    auto_memory& operator =(auto_memory&& rhs) noexcept
    {
        _mem = rhs._mem;
        rhs._mem = nullptr;
        return *this;
    }

    auto_memory(const auto_memory&) = delete;

    auto_memory& operator =(const auto_memory&) = delete;

    ~auto_memory()
    {
        if (_mem)
        {
            memory::free(_mem);
            _mem = nullptr;
        }
    }

    void* addr()
    {
        return _mem;
    }

    const void* addr() const
    {
        return _mem;
    }

private:
    void* _mem;
};
