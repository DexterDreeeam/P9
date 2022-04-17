#pragma once

#include "../../../Sdk/_Interface.hpp"

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
    ref<auto_memory>  r_mem;
};

class texture
{
    static const string file_extension;

public:
    // texture_info r_mem will be null if load_with_allocator is called directly
    template<typename Alloc_Fn_Ty>
    static texture_info load_with_allocator(const string& path, Alloc_Fn_Ty fn_alloc)
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

        void* addr = fn_alloc(header.memory_length);
        if (!f.input(addr, header.memory_length, actual_read) || actual_read != header.memory_length)
        {
            return info;
        }

        info.is_valid = boole::True;
        info.width = header.width;
        info.height = header.height;
        return info;
    }

    static texture_info load(const string& path)
    {
        ref<auto_memory> r_m;
        auto info = load_with_allocator(
            path,
            [&](sz_t sz) mutable
            {
                r_m = ref<auto_memory>::new_instance(sz);
                return r_m->addr();
            });

        if (r_m.has_value())
        {
            info.r_mem = r_m;
        }
        return info;
    }
};

_SELECTANY_ const string texture::file_extension = ".p9tt";

}
