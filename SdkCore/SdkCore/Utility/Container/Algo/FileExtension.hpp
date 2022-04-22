#pragma once

#include "../Unsafe/String.hpp"
#include "../Unsafe/Vector.hpp"

template<typename Fn_Ty>
s64 traverse_files(const string& folder_path, Fn_Ty fn)
{
    directory_cursor first_dc;
    if (!first_dc.init(folder_path.data()))
    {
        return 0;
    }
    s64 cnt = 0;
    vector<directory_cursor> vfc = { first_dc };
    while (vfc.size())
    {
        auto& dc = vfc.back();
        if (dc.is_folder())
        {
            string folder_path = "";
            folder_path += dc.directory_name();
            folder_path += dc.name();
            folder_path += '/';

            directory_cursor sub_dc;
            boole is_init = sub_dc.init(folder_path.data());
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
            if (is_init)
            {
                vfc.push_back(sub_dc);
            }
        }
        else if (dc.is_file())
        {
            string file_path;
            file_path += dc.directory_name();
            file_path += dc.name();
            fn(file_path.data());
            ++cnt;
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
        }
        else
        {
            assert(0);
        }
    }
    return cnt;
}

template<typename Fn_Ty>
vector<string> search_files(const string& folder_path, Fn_Ty fn, u64 found_max_count = 1)
{
    vector<string> rst;
    directory_cursor first_dc;
    if (!first_dc.init(folder_path.data()))
    {
        return rst;
    }
    vector<directory_cursor> vfc = { first_dc };
    while (vfc.size())
    {
        auto& dc = vfc.back();
        if (dc.is_folder())
        {
            string folder_path = "";
            folder_path += dc.directory_name();
            folder_path += dc.name();
            folder_path += '/';

            directory_cursor sub_dc;
            boole is_init = sub_dc.init(folder_path.data());
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
            if (is_init)
            {
                vfc.push_back(sub_dc);
            }
        }
        else if (dc.is_file())
        {
            string file_path;
            file_path += dc.directory_name();
            file_path += dc.name();
            if (fn(file_path.data()))
            {
                rst.push_back(file_path);
            }
            if (rst.size() == found_max_count)
            {
                break;
            }
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
        }
        else
        {
            assert(0);
        }
    }
    while (vfc.size())
    {
        vfc.back().uninit();
        vfc.pop_back();
    }
    return rst;
}

template<typename Folder_Handler_Ty, typename File_Handler_Ty>
s64 traverse_folders_files(
    const string& folder_path, Folder_Handler_Ty folder_handler, File_Handler_Ty file_handler)
{
    directory_cursor first_dc;
    if (!first_dc.init(folder_path.data()))
    {
        return 0;
    }
    s64 cnt = 0;
    vector<directory_cursor> vfc = { first_dc };
    while (vfc.size())
    {
        auto& dc = vfc.back();
        if (dc.is_folder())
        {
            string folder_path = "";
            folder_path += dc.directory_name();
            folder_path += dc.name();
            folder_path += '/';
            folder_handler(folder_path.data());
            ++cnt;
            directory_cursor sub_dc;
            boole is_init = sub_dc.init(folder_path.data());
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
            if (is_init)
            {
                vfc.push_back(sub_dc);
            }
        }
        else if (dc.is_file())
        {
            string file_path;
            file_path += dc.directory_name();
            file_path += dc.name();
            file_handler(file_path.data());
            ++cnt;
            if (!dc.move_next())
            {
                vfc.back().uninit();
                vfc.pop_back();
            }
        }
        else
        {
            assert(0);
        }
    }
    return cnt;
}
