#pragma once

class file final
{
public:
    file() :
        _ctx(nullptr)
    {
    }

    file(const file& rhs) :
        _ctx(rhs._ctx)
    {
    }

    file& operator =(const file& rhs)
    {
        _ctx = rhs._ctx;
        return *this;
    }

    ~file() = default;

public:
    static boole exist(const char* path);

    static boole create(const char* path);

    static boole remove(const char* path);

public:
    boole init_output(const char* path, boole overwrite = boole::False);

    boole init_input(const char* path);

    boole is_init() { return _ctx != nullptr; }

    boole uninit();

public:
    boole output(const char* content, s64 write_len);

    boole output(const char* content)
    {
        assert(content);
        return output(content, str_len(content));
    }

    boole input(void* buf, s64 want_read_len, s64& actual_read_len);

private:
    void* _ctx;
};
