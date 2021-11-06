
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

static class file_context
{
public:
    enum class __type : s64
    {
        None = 0,
        Output,
        Input,
    };

public:
    file_context() = delete;

    file_context(const file_context&) = delete;

    file_context(__type ty, HANDLE hndl) :
        _type(ty),
        _hndl(hndl)
    {
        assert(_type == __type::Output || _type == __type::Input);
        assert(_hndl);
    }

    ~file_context()
    {
        ::CloseHandle(_hndl);
    }

public:
    __type _type;

    HANDLE _hndl;
};

static class shadow_class_file
{
public:
    ref<file_context> _context;
};

file::file()
{
    assert(_mem_sz >= sizeof(shadow_class_file));

    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class_file*);
    shadow_self._context = ref<file_context>();
}

file::file(const file& rhs) :
    file()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class_file*);
    auto& shadow_rhs = *pointer_convert(_mem, 0, shadow_class_file*);

    shadow_self._context = shadow_rhs._context;
}

file::~file()
{
    pointer_convert(_mem, 0, shadow_class_file*)->~shadow_class_file();
}

file file::new_output(const char* path, boole overwrite)
{
    assert(path);
    s64 len = str_len(path);
    assert(len < 512);
    char path_buf[512] = {};
    memory::copy(path, path_buf, len);

    s64 idx = len;
    while (--idx >= 0)
    {
        if (path_buf[idx] == '/')
        {
            build_directory(path_buf, idx + 1);
            break;
        }
    }

    HANDLE h = ::CreateFileA(
        path,
        STANDARD_RIGHTS_WRITE | FILE_APPEND_DATA, FILE_SHARE_WRITE,
        nullptr,
        overwrite ? CREATE_ALWAYS : OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    assert(h);

    file f;
    auto& shadow = *pointer_convert(f._mem, 0, shadow_class_file*);
    shadow._context = ref<file_context>::new_instance(
        file_context::__type::Output, h);

    return f;
}

file file::new_input(const char* path)
{
    HANDLE h = ::CreateFileA(
        path, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    assert(h);

    file f;
    auto& shadow = *pointer_convert(f._mem, 0, shadow_class_file*);
    shadow._context = ref<file_context>::new_instance(
        file_context::__type::Input, h);

    return f;
}

boole file::exist(const char* path)
{
    WIN32_FIND_DATAA wfd;
    HANDLE hndl = ::FindFirstFileA(path, &wfd);
    if (hndl != INVALID_HANDLE_VALUE)
    {
        ::FindClose(hndl);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::remove(const char* path)
{
    if (::DeleteFileA(path))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

void file::build_directory(char* path, s64 len)
{
    assert(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    s64 deepest_exist_directory_idx = -1;
    s64 idx = len - 1;
    while (idx >= 0)
    {
        if (path[idx] == '/')
        {
            WIN32_FIND_DATAA wfd;
            path[idx] = 0;
            HANDLE hndl = ::FindFirstFileA(path, &wfd);
            path[idx] = '/';

            if (hndl != INVALID_HANDLE_VALUE)
            {
                ::FindClose(hndl);
                deepest_exist_directory_idx = idx;
                break;
            }
        }
        --idx;
    }

    assert(deepest_exist_directory_idx != -1);
    while (++deepest_exist_directory_idx < len)
    {
        if (path[deepest_exist_directory_idx] == '/')
        {
            path[deepest_exist_directory_idx] = 0;
            ::CreateDirectoryA(path, nullptr);
            path[deepest_exist_directory_idx] = '/';
        }
    }
}

void file::build_directory(const char* path)
{
    assert(path);
    s64 len = str_len(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    char path_buf[512] = {};
    memory::copy(path, path_buf, len);
    path_buf[len] = 0;

    build_directory(path_buf, len);
}
