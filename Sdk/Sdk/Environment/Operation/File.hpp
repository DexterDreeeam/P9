#pragma once

class file final
{
    static const u64 _mem_sz = 32;

public:
    file();

    file(const file& rhs)
    {
        memory::copy(rhs._mem, _mem, _mem_sz);
    }

    ~file();

public:
    static file new_output(const char* path);

    static file new_input(const char* path);

    static boole exist_file(const char* path);

    static boole new_file_if_not_exist(const char* path);

    static boole delete_file(const char* path);

public:
    void write(const char*);

    void read(void* buf, u64 read_len);

private:
    char _mem[_mem_sz];
};

class directory_cursor
{
    static const u64 _mem_sz = 32;

public:
    directory_cursor();

    directory_cursor(const directory_cursor& rhs)
    {
        memory::copy(rhs._mem, _mem, _mem_sz);
    }

    ~directory_cursor();

public:
    static directory_cursor create(const char* path);

private:
    char _mem[_mem_sz];
};
