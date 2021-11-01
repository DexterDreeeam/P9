#pragma once

#include "../Unsafe/String.hpp"
#include "../Unsafe/Vector.hpp"

template<typename Fn_Ty>
s64 traverse_files(const string& folder_path, Fn_Ty fn)
{
    s64 cnt = 0;
    auto first_fc = file_cursor_create(folder_path.data());
    if (!file_cursor_is_end(first_fc))
    {
        return 0;
    }
    vector<file_cursor> vfc = { first_fc };
    while (vfc.size())
    {
        auto fc = vfc.back();

        if (!file_cursor_is_end(fc))
        {
            vfc.pop_back();
            file_cursor_destroy(fc);
            if (vfc.size())
            {
                vfc.back() = file_cursor_next(vfc.back());
            }
        }
        else if (file_cursor_is_folder(fc))
        {
            vfc.push_back(file_cursor_create(fc));
        }
        else if (file_cursor_is_file(fc))
        {
            string file_path;
            file_path += file_cursor_folder_name(fc);
            file_path += file_cursor_name(fc);
            fn(file_path);
            ++cnt;
            vfc.back() = file_cursor_next(fc);
        }
        else
        {
            vfc.back() = file_cursor_next(fc);
        }
    }
    return cnt;
}

template<typename Folder_Handler_Ty, typename File_Handler_Ty>
s64 traverse_folders_files(
    const string& folder_path, Folder_Handler_Ty folder_handler, File_Handler_Ty file_handler)
{
    s64 cnt = 0;
    auto first_fc = file_cursor_create(folder_path.data());
    if (!file_cursor_is_end(first_fc))
    {
        return 0;
    }
    vector<file_cursor> vfc = { first_fc };
    while (vfc.size())
    {
        auto fc = vfc.back();

        if (!file_cursor_is_end(fc))
        {
            vfc.pop_back();
            file_cursor_destroy(fc);
            if (vfc.size())
            {
                vfc.back() = file_cursor_next(vfc.back());
            }
        }
        else if (file_cursor_is_folder(fc))
        {
            string folder_path;
            folder_path += file_cursor_folder_name(fc);
            folder_path += file_cursor_name(fc);
            folder_handler(folder_path);
            ++cnt;
            vfc.push_back(file_cursor_create(fc));
        }
        else if (file_cursor_is_file(fc))
        {
            string file_path;
            file_path += file_cursor_folder_name(fc);
            file_path += file_cursor_name(fc);
            file_handler(file_path);
            ++cnt;
            vfc.back() = file_cursor_next(fc);
        }
        else
        {
            vfc.back() = file_cursor_next(fc);
        }
    }
    return cnt;
}
