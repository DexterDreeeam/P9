
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

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

struct directory_cursor_context
{
    WIN32_FIND_DATA  _wfd;
    HANDLE           _hndl;
    char*            _folder;
};

boole directory_cursor::init(const char* directory_path)
{
    assert(directory_path);
    assert(_ctx == nullptr);

    s64 path_len = str_len(directory_path);
    assert_info(
        directory_path[path_len - 1] == '/',
        "directory path should end with \'/\'");

    char* buf = memory::alloc_copy<char>(
        directory_path, path_len + 2, path_len);
    buf[path_len] = '*';
    buf[path_len + 1] = 0;

    auto* ctx = new directory_cursor_context();
    assert(ctx);

    HANDLE h = ::FindFirstFileA(
        buf, &ctx->_wfd);

    if (h != INVALID_HANDLE_VALUE)
    {
        ctx->_hndl = h;
        ctx->_folder = buf;
        _ctx = ctx;

        if (move_next_if_invalid())
        {
            return boole::True;
        }
        else
        {
            uninit();
            return boole::False;
        }
    }
    else
    {
        memory::free(buf);
        delete ctx;
        return boole::False;
    }
}

boole directory_cursor::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_hndl);

    if (::FindClose(ctx->_hndl))
    {
        memory::free(ctx->_folder);
        delete ctx;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

const char* directory_cursor::directory_name() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);

    return ctx->_folder;
}

const char* directory_cursor::name() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);

    return ctx->_wfd.cFileName;
}

boole directory_cursor::is_file() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);

    return ctx->_wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

boole directory_cursor::is_folder() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);

    return ctx->_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

boole directory_cursor::move_next()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::FindNextFileA(
        ctx->_hndl, &ctx->_wfd))
    {
        return move_next_if_invalid();
    }
    else
    {
        return boole::False;
    }
}

boole directory_cursor::move_next_if_invalid()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (str_equal(ctx->_wfd.cFileName, ".") ||
        str_equal(ctx->_wfd.cFileName, "..") ||
        (ctx->_wfd.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY)) == 0)
    {
        // invalid
        // try move next
        return move_next();
    }
    else
    {
        return boole::True;
    }
}
