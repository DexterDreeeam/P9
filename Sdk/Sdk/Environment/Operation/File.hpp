#pragma once

class file final
{
    static const u64 _mem_sz = sizeof(ref<int>);

public:
    file();

    file(const file& rhs);

    file& operator =(const file& rhs);

    ~file();

public:
    static boole exist(const char* path);

    static boole create(const char* path);

    static boole remove(const char* path);

    static file new_output(const char* path, boole overwrite = boole::False);

    static file new_input(const char* path);

public:
    boole valid() const;

    void clear();

    boole write(const char* content, s64 write_len);

    boole write(const char* content);

    s64 read(void* buf, s64 want_read_len);

private:
    char _mem[_mem_sz];
};
