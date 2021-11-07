
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

class directory_context
{
public:
    directory_context() :
        _wfd(),
        _hndl(nullptr),
        _folder(nullptr),
        _is_end(boole::True)
    {
    }

    directory_context(const directory_context&) = delete;

    directory_context& operator =(const directory_context&) = delete;

    ~directory_context()
    {
        if (_folder)
        {
            memory::free(_folder);
        }
        if (_hndl)
        {
            ::FindClose(_hndl);
        }
    }

public:
    WIN32_FIND_DATA  _wfd;
    HANDLE           _hndl;
    char*            _folder;
    boole            _is_end;
};

class shadow_class
{
public:
    ref<directory_context> _context;
};

directory_cursor::directory_cursor()
{
    assert(_mem_sz >= sizeof(shadow_class));
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    shadow_self._context = ref<directory_context>();
}

directory_cursor::directory_cursor(const directory_cursor& rhs) :
    directory_cursor()
{
    assert(_mem_sz >= sizeof(shadow_class));
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class*);
    shadow_self._context = shadow_rhs._context;
}

directory_cursor& directory_cursor::operator =(const directory_cursor& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class*);
    shadow_self._context = shadow_rhs._context;
    return *this;
}

directory_cursor::~directory_cursor()
{
    pointer_convert(_mem, 0, shadow_class*)->~shadow_class();
}

boole directory_cursor::valid() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    return shadow._context.valid() && shadow._context->_is_end == boole::False;
}

void directory_cursor::clear()
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    shadow._context.clear();
}

const char* directory_cursor::directory_name() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());

    return shadow._context->_folder;
}

const char* directory_cursor::name() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());
    assert(shadow._context->_is_end == boole::False);

    return shadow._context->_wfd.cFileName;
}

boole directory_cursor::is_file() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());
    assert(shadow._context->_is_end == boole::False);

    return shadow._context->_wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

boole directory_cursor::is_folder() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());
    assert(shadow._context->_is_end == boole::False);

    return shadow._context->_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

void directory_cursor::move_next()
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());
    assert(shadow._context->_is_end == boole::False);

    if (::FindNextFileA(
            shadow._context->_hndl,
            &shadow._context->_wfd))
    {
        if (should_be_skipped())
        {
            move_next();
        }
    }
    else
    {
        shadow._context->_is_end = boole::True;
    }
}

boole directory_cursor::should_be_skipped() const
{
    auto& shadow = *pointer_convert(_mem, 0, shadow_class*);
    assert(shadow._context.valid());
    assert(shadow._context->_is_end == boole::False);

    if (str_equal(shadow._context->_wfd.cFileName, ".") ||
        str_equal(shadow._context->_wfd.cFileName, "..") ||
        (shadow._context->_wfd.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE + FILE_ATTRIBUTE_DIRECTORY)) == 0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

directory_cursor directory::create_cursor(const char* directory_path)
{
    assert(directory_path);

    directory_cursor cursor;
    auto& shadow = *pointer_convert(cursor._mem, 0, shadow_class*);
    shadow._context = ref<directory_context>::new_instance();

    s64 path_len = str_len(directory_path);
    assert_info(
        directory_path[path_len - 1] == '/',
        "directory path should end with \'/\'");

    char* buf = memory::alloc_copy<char>(
        directory_path, path_len + 2, path_len);
    buf[path_len] = '*';
    buf[path_len + 1] = 0;

    HANDLE h = ::FindFirstFileA(
        buf, &shadow._context->_wfd);

    if (h != INVALID_HANDLE_VALUE)
    {
        s64 path_len = str_len(directory_path);
        shadow._context->_hndl = h;
        shadow._context->_is_end = boole::False;
        shadow._context->_folder = buf;
        if (cursor.valid() && cursor.should_be_skipped())
        {
            cursor.move_next();
        }
    }
    else
    {
        memory::free(buf);
    }

    return cursor;
}

void directory::build_path(char* path, s64 len)
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

void directory::build_path(const char* path)
{
    assert(path);
    s64 len = str_len(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    char path_buf[512] = {};
    memory::copy(path, path_buf, len);
    path_buf[len] = 0;

    build_path(path_buf, len);
}
