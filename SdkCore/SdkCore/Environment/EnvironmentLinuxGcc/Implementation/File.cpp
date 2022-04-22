
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

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

    file_context(__type ty, int fp) :
        _type(ty),
        _fp(fp)
    {
        assert(_type == __type::Output || _type == __type::Input);
        assert(_fp);
    }

    ~file_context() = default;
public:
    __type _type;
    int    _fp;
};

boole file::exist(const char* path)
{
    return ::access(path, F_OK) == 0;
}

boole file::create(const char* path)
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
            directory::build_path(path_buf, idx + 1);
            break;
        }
    }
    auto file = ::open(
        path, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (file < 0)
    {
        return boole::False;
    }
    ::close(file);
    return boole::True;
}

boole file::remove(const char* path)
{
    if (::remove(path) >= 0)
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

    auto flag = O_CREAT | O_WRONLY;
    flag |= overwrite ? O_TRUNC : O_APPEND;

    auto f = ::open(path, flag, S_IRWXU | S_IRWXG | S_IRWXO);
    if (f >= 0)
    {
        auto* ctx = new file_context(file_context::__type::Output, f);
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

    auto flag = O_RDONLY;
    auto f = ::open(path, flag);
    if (f >= 0)
    {
        auto* ctx = new file_context(file_context::__type::Input, f);
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
    assert(ctx->_fp);

    if (::close(ctx->_fp) >= 0)
    {
        ctx->_type = file_context::__type::None;
        ctx->_fp = 0;
        delete ctx;
        _ctx = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::output(const void* content, s64 write_len)
{
    auto* ctx = pointer_convert(_ctx, 0, file_context*);
    assert(ctx);
    assert(ctx->_type == file_context::__type::Output);
    assert(ctx->_fp);
    assert(content);

    if (::write(
        ctx->_fp, content, write_len) >= 0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::input(void* buf, s64 want_read_len, s64& actual_read_len)
{
    auto* ctx = pointer_convert(_ctx, 0, file_context*);
    assert(ctx);
    assert(ctx->_type == file_context::__type::Input);
    assert(ctx->_fp);
    assert(buf);

    s64 read_len = ::read(
        ctx->_fp, buf, want_read_len);

    if (read_len > 0)
    {
        actual_read_len = read_len;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
