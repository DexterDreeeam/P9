#pragma once

class directory final
{
public:
    static void build_path(char* path, s64 len);

    static void build_path(const char* path);
};

class directory_cursor final
{
    friend class directory;

public:
    directory_cursor() :
        _ctx(nullptr)
    {
    }

    directory_cursor(const directory_cursor& rhs) :
        _ctx(rhs._ctx)
    {
    }

    directory_cursor& operator =(const directory_cursor& rhs)
    {
        _ctx = rhs._ctx;
    }

    ~directory_cursor() = default;

public:
    boole init(const char* directory_path);

    boole uninit();

    const char* directory_name() const;

    const char* name() const;

    boole is_file() const;

    boole is_folder() const;

    boole move_next();

private:
    // invalid file or folder, include 'hidden file', 'link file'm '.', '..'
    // return true  if 'valid' or 'move next' successfully
    // return false if 'invalid' and couldn't 'move next'
    boole move_next_if_invalid();

private:
    void* _ctx;
};
