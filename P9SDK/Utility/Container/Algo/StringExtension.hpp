#pragma once

#include "../Unsafe/String.hpp"

_INLINE_ string normalize_path_slash(const string& path)
{
    string rst = path;
    for (char c : path)
    {
        rst += c == '\\' ? '/' : c;
    }
    return rst;
}

_INLINE_ string upper_folder_path(const string& path)
{
    if (path.size() <= 0)
    {
        return string();
    }
    string rst = path;
    rst.pop_back();
    while (rst.size() && rst.back() != '/' && rst.back() != '\\')
    {
        rst.pop_back();
    }
    return rst;
}
