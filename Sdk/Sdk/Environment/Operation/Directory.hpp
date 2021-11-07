#pragma once

class directory_cursor final
{
    friend class directory;

    static constexpr u64 _mem_sz = sizeof(ref<int>);

public:
    directory_cursor();

    directory_cursor(const directory_cursor& rhs);

    directory_cursor& operator =(const directory_cursor& rhs);

    ~directory_cursor();

public:
    boole valid() const;

    boole invalid() const
    {
        return !valid();
    }

    void clear();

    const char* directory_name() const;

    const char* name() const;

    boole is_file() const;

    boole is_folder() const;

    void move_next();

private:
    // file or directory, except 'hidden file', 'link file'm '.', '..'
    boole should_be_skipped() const;

private:
    char _mem[_mem_sz];
};

class directory final
{
public:
    static directory_cursor create_cursor(const char* directory_path);

    static void build_path(char* path, s64 len);

    static void build_path(const char* path);
};
