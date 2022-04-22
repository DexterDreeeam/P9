
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

boole directory::exist(const char* path)
{
    struct stat st;
    if(::stat(path, &st) == 0 && (st.st_mode & S_IFDIR) != 0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

void directory::build_path(char* path, s64 len)
{
    assert(path);
    assert_info(path[len - 1] == '/', "directory path should end with \'/\'");

    s64 first_inexist_directory_idx = -1;
    s64 idx = len - 1;
    while (idx >= 0)
    {
        if (path[idx] == '/')
        {
            char old_ch = path[idx + 1];
            path[idx + 1] = 0;
            boole directory_exist = exist(path);
            path[idx + 1] = old_ch;
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
        while (idx < len && path[idx] != '/')
        {
            ++idx;
        }
        if (idx >= len)
        {
            break;
        }
        path[idx] = 0;
        ::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
        path[idx] = '/';
        first_inexist_directory_idx = idx;
    }
}

struct directory_cursor_context
{
    DIR*             _dir;
    dirent*          _dir_ptr;
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

    DIR* dir = ::opendir(directory_path);
    if (dir == nullptr)
    {
        return boole::False;
    }

    auto* ctx = new directory_cursor_context();
    ctx->_dir = dir;
    ctx->_dir_ptr = readdir(dir);
    if (ctx->_dir_ptr == nullptr)
    {
        ::closedir(dir);
        delete ctx;
        return boole::False;
    }
    ctx->_folder = memory::alloc_copy<char>(
        directory_path, path_len + 1, path_len + 1);

    _ctx = ctx;
    if (valid_or_move_next())
    {
        // valid or move
        return boole::True;
    }
    else
    {
        uninit();
        return boole::False;
    }
}

boole directory_cursor::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (::closedir(ctx->_dir))
    {
        memory::free(ctx->_folder);
        delete ctx;
        _ctx = nullptr;
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
    assert(ctx->_dir);

    return ctx->_folder;
}

const char* directory_cursor::name() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_name;
}

boole directory_cursor::is_file() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_type == DT_REG;
}

boole directory_cursor::is_folder() const
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    return ctx->_dir_ptr->d_type == DT_DIR;
}

boole directory_cursor::move_next()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (ctx->_dir_ptr)
    {
        ctx->_dir_ptr = ::readdir(ctx->_dir);
        return valid_or_move_next();
    }
    else
    {
        return boole::False;
    }
}

boole directory_cursor::valid_or_move_next()
{
    auto* ctx = pointer_convert(_ctx, 0, directory_cursor_context*);
    assert(ctx);
    assert(ctx->_folder);
    assert(ctx->_dir);

    if (ctx->_dir_ptr == nullptr)
    {
        return boole::False;
    }
    else if (
        str_equal(ctx->_dir_ptr->d_name, ".") ||
        str_equal(ctx->_dir_ptr->d_name, "..") ||
        (ctx->_dir_ptr->d_type != DT_REG && ctx->_dir_ptr->d_type != DT_DIR))
    {
        // invalid
        // try move next
        return move_next();
    }
    else
    {
        // normal file or folder
        return boole::True;
    }
}
