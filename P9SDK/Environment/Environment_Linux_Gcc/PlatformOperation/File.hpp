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

struct _file_cursor
{
    DIR*     _dir;
    dirent*  _dir_ptr;
    char*    _folder;
};

typedef void*  file_cursor;

_INLINE_ outf  output_file_create(const char* path, boole overwrite = boole::False);
_INLINE_ boole output_file_write(outf x, const char* content);
_INLINE_ boole output_file_write(outf x, const char* content, s64 write_len);
_INLINE_ boole output_file_destroy(outf x);

_INLINE_ inpf  input_file_create(const char* path);
_INLINE_ s64   input_file_read(inpf x, void* buf, s64 want_read);
_INLINE_ boole input_file_destroy(inpf x);

_INLINE_ boole create_file(const char* path);
_INLINE_ boole delete_file(const char* path);
_INLINE_ boole is_file_exist(const char* path);

_INLINE_ boole is_directory_exist(const char* path);
_INLINE_ boole create_directory(const char* path);

_INLINE_ file_cursor  file_cursor_create(const char* directory_path);
_INLINE_ file_cursor  file_cursor_create(file_cursor x);
_INLINE_ const char*  file_cursor_name(file_cursor x);
_INLINE_ const char*  file_cursor_folder_name(file_cursor x);
_INLINE_ boole        file_cursor_is_end(file_cursor x);
_INLINE_ boole        file_cursor_is_file(file_cursor x);
_INLINE_ boole        file_cursor_is_folder(file_cursor x);
_INLINE_ file_cursor  file_cursor_next(file_cursor x);
_INLINE_ void         file_cursor_destroy(file_cursor x);

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
    assert(f >= 0);
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
    assert(f >= 0);
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

_INLINE_ boole is_file_exist(const char* path)
{
    return !access(path, F_OK);
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

_INLINE_ file_cursor file_cursor_create(const char* directory_path)
{
    DIR* dir = opendir(directory_path);
    if (dir == nullptr)
    {
        return (file_cursor)nullptr;
    }

    _file_cursor* cursor = new _file_cursor();
    cursor->_dir = dir;
    cursor->_dir_ptr = readdir(dir);
    if (cursor->_dir_ptr == nullptr)
    {
        closedir(dir);
        delete cursor;
        return (file_cursor)nullptr;
    }

    s64 path_len = str_len(directory_path);
    cursor->_folder = (char*)memory_alloc_copy(
        directory_path, path_len + 1, path_len + 1);

    return (file_cursor)cursor;
}

_INLINE_ file_cursor file_cursor_create(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);
    assert(file_cursor_is_folder(x));

    s64 parent_folder_len = str_len(cursor->_folder);
    s64 folder_len = str_len(cursor->_dir_ptr->d_name);

    char* buf = new char[parent_folder_len + folder_len + 2];
    memory_copy(cursor->_folder, buf, parent_folder_len);
    memory_copy(cursor->_dir_ptr->d_name, buf + parent_folder_len, folder_len);
    buf[parent_folder_len + folder_len] = '/';
    buf[parent_folder_len + folder_len + 1] = 0;

    file_cursor sub_cursor = file_cursor_create(buf);
    delete[] buf;
    return sub_cursor;
}

_INLINE_ const char*  file_cursor_name(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);

    return cursor->_dir_ptr->d_name;
}

_INLINE_ const char*  file_cursor_folder_name(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);

    return cursor->_folder;
}

_INLINE_ boole file_cursor_is_end(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;

    return cursor != nullptr && cursor->_dir_ptr != nullptr;
}

_INLINE_ boole file_cursor_is_file(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);

    return cursor->_dir_ptr->d_type == DT_REG;
}

_INLINE_ boole file_cursor_is_folder(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);

    if (str_equal(cursor->_dir_ptr->d_name, ".") ||
        str_equal(cursor->_dir_ptr->d_name, ".."))
    {
        return boole::False;
    }
    else
    {
        return cursor->_dir_ptr->d_type == DT_DIR;
    }
}

_INLINE_ file_cursor file_cursor_next(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_dir_ptr != nullptr);

    cursor->_dir_ptr = readdir(cursor->_dir);
    return x;
}

_INLINE_ void file_cursor_destroy(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor);

    memory_free(cursor->_folder);
    closedir(cursor->_dir);
    delete cursor;
}
