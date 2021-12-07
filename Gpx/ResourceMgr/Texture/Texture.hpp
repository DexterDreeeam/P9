#pragma once

#include "../../../Sdk/Interface.hpp"

namespace gpx
{

struct texture_header
{
    s64 width;
    s64 height;
    s64 memory_length;
};

struct texture_info
{
    boole             is_valid;
    s64               width;
    s64               height;
    ref<auto_memory>  memory;
};

class texture
{
    static const string file_extension;

public:
    static texture_info load(const string& path)
    {
        texture_info info = {};
        info.is_valid = boole::False;

        if (!path.end_with(file_extension))
        {
            return info;
        }

        file f;
        if (!f.init_input(path.data()))
        {
            return info;
        }
        escape_function ef =
            [=]() mutable
            {
                f.uninit();
            };

        texture_header header = {};
        s64 actual_read;
        if (!f.input(&header, sizeof(header), actual_read) || actual_read != sizeof(header))
        {
            return info;
        }

        auto r_mem = ref<auto_memory>::new_instance(header.memory_length);
        if (!f.input(r_mem->addr(), header.memory_length, actual_read) || actual_read != header.memory_length)
        {
            return info;
        }

        info.is_valid = boole::True;
        info.width = header.width;
        info.height = header.height;
        info.memory = r_mem;
        return info;
    }
};

_SELECTANY_ const string texture::file_extension = ".p9tt";

}
