#pragma once

class file final
{
    static const u64 _mem_sz = 48;

public:
    file();

    file(const file& rhs);

    ~file();

public:
    static void build_directory(char* path, s64 len);

    static void build_directory(const char* path);

    static file new_output(const char* path, boole overwrite = boole::False);

    static file new_input(const char* path);

    static boole exist(const char* path);

    static boole remove(const char* path);

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

    directory_cursor(const directory_cursor& rhs);

    ~directory_cursor();

public:
    static directory_cursor create(const char* path);

private:
    char _mem[_mem_sz];
};
