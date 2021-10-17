#pragma once

#include "Memory.hpp"

typedef  void* outf;
typedef  void* inpf;

struct _outf
{
    decltype(open(nullptr, 0)) _fp;
};

struct _inpf
{
    decltype(open(nullptr, 0)) _fp;
};

_INLINE_ outf  output_file_create(const char* path, boole overwrite = boole::False);
_INLINE_ boole output_file_write(outf x, const char* content);
_INLINE_ boole output_file_write(outf x, const char* content, s64 write_len);
_INLINE_ boole output_file_destroy(outf x);

_INLINE_ inpf  input_file_create(const char* path);
_INLINE_ s64   input_file_read(inpf x, void* buf, s64 want_read);
_INLINE_ boole input_file_destroy(inpf x);

_INLINE_ boole create_file(const char* path);
_INLINE_ boole delete_file(const char* path);
_INLINE_ boole is_directory_exist(const char* path);
_INLINE_ boole create_directory(const char* path);

_INLINE_ outf output_file_create(const char *path, boole overwrite)
{
    char path_buf[512] = {};
    s64 len = str_len(path);
    assert(len < 512);
    memory_copy(path, path_buf, len);
    s64 first_inexist_directory_idx = -1;
    s64 idx = len - 1;
    while (idx >= 0)
    {
        if (path_buf[idx] == '/')
        {
            path_buf[idx] = 0;
            boole directory_exist = is_directory_exist(path_buf);
            path_buf[idx] = '/';
            if (directory_exist)
            {
                first_inexist_directory_idx = idx;
                break;
            }
        }
        --idx;
    }
    assert(first_inexist_directory_idx >= 0);
    while (first_inexist_directory_idx < len)
    {
        idx = first_inexist_directory_idx + 1;
        while (idx < len && path_buf[idx] != '/')
        {
            ++idx;
        }
        if (idx >= len)
        {
            break;
        }
        path_buf[idx] = 0;
        create_directory(path_buf);
        path_buf[idx] = '/';
        first_inexist_directory_idx = idx;
    }
    auto flag = O_CREAT | O_WRONLY;
    if (overwrite)
    {
        flag |= O_TRUNC;
    }
    else
    {
        flag |= O_APPEND;
    }
    auto f = open(path, flag, S_IRWXU | S_IRWXG | S_IRWXO);
    _outf* ret = new _outf();
    ret->_fp = f;
    return (outf)ret;
}

_INLINE_ boole output_file_write(outf x, const char *content)
{
    return output_file_write(x, content, str_len(content));
}

_INLINE_ boole output_file_write(outf x, const char *content, s64 write_len)
{
    _outf* f = (_outf*)x;
    assert(f && f->_fp);
    return write(f->_fp, content, write_len) >= 0;
}

_INLINE_ boole output_file_destroy(outf x)
{
    _outf* f = (_outf*)x;
    assert(f && f->_fp);
    boole rst = close(f->_fp) >= 0;
    delete f;
    return rst;
}

_INLINE_ inpf input_file_create(const char *path)
{
    auto flag = O_RDONLY;
    auto f = open(path, flag);
    _inpf* ret = new _inpf();
    ret->_fp = f;
    return (inpf)ret;
}

_INLINE_ s64 input_file_read(inpf x, void *buf, s64 want_read)
{
    _inpf* f = (_inpf*)x;
    assert(f && f->_fp);
    s64 actual_read = read(f->_fp, buf, want_read);
    assert(actual_read >= 0);
    return actual_read;
}

_INLINE_ boole input_file_destroy(inpf x)
{
    _inpf* f = (_inpf*)x;
    assert(f && f->_fp);
    boole rst = close(f->_fp) >= 0;
    delete f;
    return rst;
}

_INLINE_ boole create_file(const char* path)
{
    auto file = open(path, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (file < 0)
    {
        return boole::False;
    }
    close(file);
    return boole::True;
}

_INLINE_ boole delete_file(const char *path)
{
    if (remove(path) >= 0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ boole is_directory_exist(const char* path)
{
    struct stat st;
    if(stat(path, &st) == 0 && (st.st_mode & S_IFDIR) != 0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ boole create_directory(const char* path)
{
    return mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == 0;
}
