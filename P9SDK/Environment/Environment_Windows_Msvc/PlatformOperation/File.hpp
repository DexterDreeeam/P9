#pragma once

#include "Memory.hpp"

typedef  hndl  outf;
typedef  hndl  inpf;

struct _file_cursor
{
    WindowsMsvcNs::WIN32_FIND_DATA  _wfd;
    WindowsMsvcNs::HANDLE           _hndl;
    char*                           _folder;
    boole                           _is_end;
};

typedef void*  file_cursor;

_INLINE_ outf  output_file_create(const char* path, boole overwrite = boole::False);
_INLINE_ boole output_file_write(outf f, const char* content);
_INLINE_ boole output_file_write(outf f, const char* content, s64 write_len);
_INLINE_ boole output_file_destroy(outf f);

_INLINE_ inpf  input_file_create(const char* path);
_INLINE_ s64   input_file_read(inpf f, void* buf, s64 want_read);
_INLINE_ boole input_file_destroy(inpf f);

_INLINE_ boole create_file(const char* path);
_INLINE_ boole delete_file(const char* path);
_INLINE_ boole is_file_exist(const char* path);

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
    s64 last_inexist_directory_idx = 0;
    s64 idx = len - 1;
    WindowsMsvcNs::WIN32_FIND_DATAA FindFileData;
    while (idx >= 0)
    {
        if (path_buf[idx] == '/')
        {
            path_buf[idx] = 0;
            WindowsMsvcNs::HANDLE hFind = WindowsMsvcNs::FindFirstFileA(path_buf, &FindFileData);
            path_buf[idx] = '/';
            if (hFind != (WindowsMsvcNs::HANDLE)(WindowsMsvcNs::LONG_PTR)-1)
            {
                //# not INVALID_HANDLE_VALUE
                WindowsMsvcNs::FindClose(hFind);
                last_inexist_directory_idx = idx;
                break;
            }
        }
        --idx;
    }
    assert(last_inexist_directory_idx);
    while (last_inexist_directory_idx < len)
    {
        idx = last_inexist_directory_idx + 1;
        while (idx < len && path_buf[idx] != '/')
        {
            ++idx;
        }
        if (idx >= len)
        {
            break;
        }
        path_buf[idx] = 0;
        if (!WindowsMsvcNs::CreateDirectoryA(path_buf, nullptr))
        {
            assert(0);
        }
        path_buf[idx] = '/';
        last_inexist_directory_idx = idx;
    }
    outf f = (outf)WindowsMsvcNs::CreateFileA(
        path,
        STANDARD_RIGHTS_WRITE | FILE_APPEND_DATA, FILE_SHARE_WRITE,
        nullptr,
        overwrite ? CREATE_ALWAYS : OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);
    return f;
}

_INLINE_ boole output_file_write(outf f, const char *content)
{
    if (WindowsMsvcNs::WriteFile((WindowsMsvcNs::HANDLE)f, content, (WindowsMsvcNs::DWORD)str_len(content), nullptr, nullptr))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ boole output_file_write(outf f, const char *content, s64 write_len)
{
    if (WindowsMsvcNs::WriteFile((WindowsMsvcNs::HANDLE)f, content, (WindowsMsvcNs::DWORD)write_len, nullptr, nullptr))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ boole output_file_destroy(outf f)
{
    WindowsMsvcNs::FlushFileBuffers((WindowsMsvcNs::HANDLE)f);
    if (WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)f))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ inpf input_file_create(const char *path)
{
    inpf f = (inpf)WindowsMsvcNs::CreateFileA(
        path, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    assert(f);
    return f;
}

_INLINE_ s64 input_file_read(inpf f, void *buf, s64 want_read)
{
    u32 actual_read = 0;
    if (WindowsMsvcNs::ReadFile((WindowsMsvcNs::HANDLE)f, buf, (WindowsMsvcNs::DWORD)want_read, &actual_read, nullptr) == 0)
    {
        assert(0);
    }
    return actual_read;
}

_INLINE_ boole input_file_destroy(inpf f)
{
    if (WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)f))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

using namespace WindowsMsvcNs;
_INLINE_ boole create_file(const char* path)
{
    auto h = CreateFile(
        path, STANDARD_RIGHTS_WRITE, FILE_SHARE_WRITE, nullptr,
        CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h == INVALID_HANDLE_VALUE)
    {
        return boole::False;
    }
    CloseHandle(h);
    return boole::True;
}

_INLINE_ boole delete_file(const char *path)
{
    if (WindowsMsvcNs::DeleteFileA(path))
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
    WindowsMsvcNs::WIN32_FIND_DATAA FindFileData;
    WindowsMsvcNs::HANDLE hFind = WindowsMsvcNs::FindFirstFileA(path, &FindFileData);
    if (hFind != (WindowsMsvcNs::HANDLE)(WindowsMsvcNs::LONG_PTR)-1)
    {
        //# not INVALID_HANDLE_VALUE
        WindowsMsvcNs::FindClose(hFind);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ file_cursor file_cursor_create(const char* directory_path)
{
    using namespace WindowsMsvcNs;

    _file_cursor* cursor = new _file_cursor();
    cursor->_is_end = boole::False;

    s64 path_len = str_len(directory_path);
    cursor->_folder = (char*)memory_alloc_copy(
        directory_path, path_len + 2, path_len);

    cursor->_folder[path_len++] = '*';
    cursor->_folder[path_len++] = 0;

    cursor->_hndl = WindowsMsvcNs::FindFirstFileA(cursor->_folder, &cursor->_wfd);
    cursor->_folder[path_len - 2] = 0;

    if (cursor->_hndl != INVALID_HANDLE_VALUE)
    {
        return (file_cursor)cursor;
    }
    else
    {
        delete cursor;
        return (file_cursor)nullptr;
    }
}

_INLINE_ file_cursor file_cursor_create(file_cursor x)
{
    using namespace WindowsMsvcNs;

    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);
    assert(file_cursor_is_folder(x));

    s64 parent_folder_len = str_len(cursor->_folder);
    s64 folder_len = str_len(cursor->_wfd.cFileName);

    char* buf = new char[parent_folder_len + folder_len + 2];
    memory_copy(cursor->_folder, buf, parent_folder_len);
    memory_copy(cursor->_wfd.cFileName, buf + parent_folder_len, folder_len);
    buf[parent_folder_len + folder_len] = '/';
    buf[parent_folder_len + folder_len + 1] = 0;

    file_cursor sub_cursor = file_cursor_create(buf);
    delete[] buf;
    return sub_cursor;
}

_INLINE_ const char*  file_cursor_name(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);

    return cursor->_wfd.cFileName;
}

_INLINE_ const char*  file_cursor_folder_name(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);

    return cursor->_folder;
}

_INLINE_ boole file_cursor_is_end(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;

    return cursor != nullptr && cursor->_is_end == boole::False;
}

_INLINE_ boole file_cursor_is_file(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);

    return cursor->_wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

_INLINE_ boole file_cursor_is_folder(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);

    if (str_equal(cursor->_wfd.cFileName, ".") ||
        str_equal(cursor->_wfd.cFileName, ".."))
    {
        return boole::False;
    }
    else
    {
        return cursor->_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    }
}

_INLINE_ file_cursor file_cursor_next(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor && cursor->_is_end == boole::False);

    if (WindowsMsvcNs::FindNextFileA(cursor->_hndl, &cursor->_wfd))
    {
        ;
    }
    else
    {
        cursor->_is_end = boole::True;
    }
    return x;
}

_INLINE_ void file_cursor_destroy(file_cursor x)
{
    _file_cursor* cursor = (_file_cursor*)x;
    assert(cursor);

    memory_free(cursor->_folder);
    WindowsMsvcNs::FindClose(cursor->_hndl);
    delete cursor;
}
