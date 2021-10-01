#pragma once

#include "Memory.h"

typedef  hndl  outf;
typedef  hndl  inpf;

_INLINE_ outf  output_file_create(const char* path);
_INLINE_ boole output_file_write(outf f, const char* content);
_INLINE_ boole output_file_write(outf f, const char* content, s64 write_len);
_INLINE_ boole output_file_destroy(outf f);
_INLINE_ inpf  input_file_create(const char* path);
_INLINE_ s64   input_file_read(inpf f, void* buf, s64 want_read);
_INLINE_ boole input_file_destroy(inpf f);
_INLINE_ boole delete_file(const char* path);

_INLINE_ outf output_file_create(const char *path)
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
        path, STANDARD_RIGHTS_WRITE | FILE_APPEND_DATA, FILE_SHARE_WRITE, nullptr,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    return f;
}

_INLINE_ boole output_file_write(outf f, const char *content)
{
    if (WindowsMsvcNs::WriteFile((WindowsMsvcNs::HANDLE)f, content, (WindowsMsvcNs::DWORD)str_len(content), nullptr, nullptr))
    {
        return boole_true;
    }
    else
    {
        return boole_false;
    }
}

_INLINE_ boole output_file_write(outf f, const char *content, s64 write_len)
{
    if (WindowsMsvcNs::WriteFile((WindowsMsvcNs::HANDLE)f, content, (WindowsMsvcNs::DWORD)write_len, nullptr, nullptr))
    {
        return boole_true;
    }
    else
    {
        return boole_false;
    }
}

_INLINE_ boole output_file_destroy(outf f)
{
    WindowsMsvcNs::FlushFileBuffers((WindowsMsvcNs::HANDLE)f);
    if (WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)f))
    {
        return boole_true;
    }
    else
    {
        return boole_false;
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
        return boole_true;
    }
    else
    {
        return boole_false;
    }
}

_INLINE_ boole delete_file(const char *path)
{
    if (WindowsMsvcNs::DeleteFileA(path))
    {
        return boole_true;
    }
    else
    {
        return boole_false;
    }
}
