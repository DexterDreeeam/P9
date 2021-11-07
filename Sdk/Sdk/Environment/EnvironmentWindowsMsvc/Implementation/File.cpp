
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
    const __type _type;

    const HANDLE _hndl;
};

class shadow_class
{
public:
    ref<file_context> _context;
};

file::file()
{
    assert(_mem_sz >= sizeof(shadow_class));
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    shadow_self._context = ref<file_context>();
}

file::file(const file& rhs) :
    file()
{
    assert(_mem_sz >= sizeof(shadow_class));
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class*);
    shadow_self._context = shadow_rhs._context;
}

file& file::operator =(const file& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class*);
    shadow_self._context = shadow_rhs._context;
    return *this;
}

file::~file()
{
    pointer_convert(_mem, 0, shadow_class*)->~shadow_class();
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
            directory::build_path(path_buf, idx + 1);
            break;
        }
    }

    file f;
    HANDLE h = ::CreateFileA(
        path,
        STANDARD_RIGHTS_WRITE | FILE_APPEND_DATA, FILE_SHARE_WRITE,
        nullptr,
        overwrite ? CREATE_ALWAYS : OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (h)
    {
        auto& shadow = *pointer_convert(f._mem, 0, shadow_class*);
        shadow._context = ref<file_context>::new_instance(
            file_context::__type::Output, h);
    }
    return f;
}

file file::new_input(const char* path)
{
    file f;
    HANDLE h = ::CreateFileA(
        path, FILE_GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h)
    {
        auto& shadow = *pointer_convert(f._mem, 0, shadow_class*);
        shadow._context = ref<file_context>::new_instance(
            file_context::__type::Input, h);
    }
    return f;
}

boole file::valid() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    return shadow._context.valid();
}

void file::clear()
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    return shadow._context.clear();
}

boole file::write(const char* content, s64 write_len)
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(content);
    assert(shadow._context.valid());
    assert(shadow._context->_type == file_context::__type::Output);

    if (::WriteFile(
        shadow._context->_hndl, content, write_len, nullptr, nullptr))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole file::write(const char* content)
{
    assert(content);
    return write(content, str_len(content));
}

s64 file::read(void* buf, s64 want_read_len)
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(buf);
    assert(shadow._context.valid());
    assert(shadow._context->_type == file_context::__type::Input);

    u32 actual_read_len = 0;
    ::ReadFile(
        shadow._context->_hndl, buf, want_read_len, &actual_read_len, nullptr);

    return (s64)actual_read_len;
}
