#pragma once

class file final
{
public:
    file();

    file(const file& rhs);

    file& operator =(const file& rhs);

    ~file() = default;

public:
    static boole exist(const char* path);

    static boole create(const char* path);

    static boole remove(const char* path);

public:
    boole init_output(const char* path, boole overwrite = boole::False);

    boole init_input(const char* path);

    boole is_init();

    boole uninit();

public:
    boole output(const void* content, s64 write_len);

    boole output(const char* content);

    boole input(void* buf, s64 want_read_len, s64& actual_read_len);

private:
    void* _ctx;
};
