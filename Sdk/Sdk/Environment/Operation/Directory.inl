#pragma once

_INLINE_ void directory::build_path(const char* path)
{
    assert(path);
    s64 len = str_len(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    char path_buf[512] = {};
    memory::copy(path, path_buf, len);
    path_buf[len] = 0;

    build_path(path_buf, len);
}

_INLINE_ directory_cursor::directory_cursor() :
    _ctx(nullptr)
{
}

_INLINE_ directory_cursor::directory_cursor(const directory_cursor& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ directory_cursor& directory_cursor::operator =(const directory_cursor& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}
