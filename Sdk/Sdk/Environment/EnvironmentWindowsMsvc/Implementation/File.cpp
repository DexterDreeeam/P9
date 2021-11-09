
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

class file_context
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
        if (_type == __type::Output)
        {
            // flush potential buffered content
            ::FlushFileBuffers(_hndl);
        }
        ::CloseHandle(_hndl);
    }

public:
    __type _type;

    HANDLE _hndl;
};

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

boole file::create(const char* path)
{
    auto h = ::CreateFileA(
        path, STANDARD_RIGHTS_WRITE, FILE_SHARE_WRITE, nullptr,
        CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h == INVALID_HANDLE_VALUE)
    {
        return boole::False;
    }
    ::CloseHandle(h);
    return boole::True;
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

boole file::init_output(const char* path, boole overwrite)
{
    assert(_ctx == nullptr);
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
            directory::build_path(path_buf, idx + 1);
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

    if (h)
    {
        auto* ctx = new file_context(file_context::__type::Output, h);
        _ctx = ctx;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::init_input(const char* path)
{
    assert(_ctx == nullptr);
    assert(path);

    HANDLE h = ::CreateFileA(
        path, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h)
    {
        auto* ctx = new file_context(file_context::__type::Input, h);
        _ctx = ctx;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, file_context*);
    assert(ctx);
    assert(ctx->_type == file_context::__type::Output || ctx->_type == file_context::__type::Input);
    assert(ctx->_hndl);

    if (::CloseHandle(ctx->_hndl))
    {
        ctx->_type = file_context::__type::None;
        ctx->_hndl = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }

}

boole file::output(const char* content, s64 write_len)
{
    auto* ctx = pointer_convert(_ctx, 0, file_context*);
    assert(ctx);
    assert(ctx->_type == file_context::__type::Output);
    assert(ctx->_hndl);
    assert(content);

    if (::WriteFile(
        ctx->_hndl, content, write_len, nullptr, nullptr))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::output(const char* content)
{
    assert(content);
    return output(content, str_len(content));
}

boole file::input(void* buf, s64 want_read_len, s64& actual_read_len)
{
    auto* ctx = pointer_convert(_ctx, 0, file_context*);
    assert(ctx);
    assert(ctx->_type == file_context::__type::Input);
    assert(ctx->_hndl);
    assert(buf);

    u32 read_len = 0;

    if (::ReadFile(
        ctx->_hndl, buf, want_read_len, &read_len, nullptr))
    {
        actual_read_len = (s64)read_len;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
