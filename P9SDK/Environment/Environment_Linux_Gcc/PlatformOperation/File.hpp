#pragma once

#include "Memory.hpp"

typedef  hndl  outf;
typedef  hndl  inpf;

_INLINE_ outf  output_file_create(const char* path, boole overwrite = boole::False);
_INLINE_ boole output_file_write(outf f, const char* content);
_INLINE_ boole output_file_write(outf f, const char* content, s64 write_len);
_INLINE_ boole output_file_destroy(outf f);
_INLINE_ inpf  input_file_create(const char* path);
_INLINE_ s64   input_file_read(inpf f, void* buf, s64 want_read);
_INLINE_ boole input_file_destroy(inpf f);
namespace
{
_INLINE_ boole create_file(const char* path);
}
_INLINE_ boole delete_file(const char* path);


_INLINE_ outf output_file_create(const char *path, boole overwrite)
{
    char path_buf[512] = {};
    s64 len = str_len(path);
    assert(len < 512);
    memory_copy(path, path_buf, len);
    s64 last_inexist_directory_idx = 0;
    s64 idx = len - 1;
    LinuxGccNs::WIN32_FIND_DATAA FindFileData;
    while (idx >= 0)
    {
        if (path_buf[idx] == '/')
        {
            path_buf[idx] = 0;
            LinuxGccNs::HANDLE hFind = LinuxGccNs::FindFirstFileA(path_buf, &FindFileData);
            path_buf[idx] = '/';
            if (hFind != (LinuxGccNs::HANDLE)(LinuxGccNs::LONG_PTR)-1)
            {
                //# not INVALID_HANDLE_VALUE
                LinuxGccNs::FindClose(hFind);
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
        if (!LinuxGccNs::CreateDirectoryA(path_buf, nullptr))
        {
            assert(0);
        }
        path_buf[idx] = '/';
        last_inexist_directory_idx = idx;
    }
    outf f = (outf)LinuxGccNs::CreateFileA(
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
    if (LinuxGccNs::WriteFile((LinuxGccNs::HANDLE)f, content, (LinuxGccNs::DWORD)str_len(content), nullptr, nullptr))
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
    if (LinuxGccNs::WriteFile((LinuxGccNs::HANDLE)f, content, (LinuxGccNs::DWORD)write_len, nullptr, nullptr))
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
    LinuxGccNs::FlushFileBuffers((LinuxGccNs::HANDLE)f);
    if (LinuxGccNs::CloseHandle((LinuxGccNs::HANDLE)f))
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
    inpf f = (inpf)LinuxGccNs::CreateFileA(
        path, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    assert(f);
    return f;
}

_INLINE_ s64 input_file_read(inpf f, void *buf, s64 want_read)
{
    u32 actual_read = 0;
    if (LinuxGccNs::ReadFile((LinuxGccNs::HANDLE)f, buf, (LinuxGccNs::DWORD)want_read, &actual_read, nullptr) == 0)
    {
        assert(0);
    }
    return actual_read;
}

_INLINE_ boole input_file_destroy(inpf f)
{
    if (LinuxGccNs::CloseHandle((LinuxGccNs::HANDLE)f))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

namespace
{

using namespace LinuxGccNs;
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

}

_INLINE_ boole delete_file(const char *path)
{
    if (LinuxGccNs::DeleteFileA(path))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
