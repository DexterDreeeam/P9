#pragma once

namespace gpx
{

_INLINE_ vertex_type vertices_buffer_pos_color::vertex_type()
{
    return vertex_pos_color::type();
}

_INLINE_ s64 vertices_buffer_pos_color::vertex_count()
{
    return _vertices.size();
}

_INLINE_ s64 vertices_buffer_pos_color::vertex_data_size()
{
    return sizeof(vertex_pos_color);
}

_INLINE_ s64 vertices_buffer_pos_color::vertices_data_size()
{
    return vertex_data_size() * vertex_count();
}

_INLINE_ void* vertices_buffer_pos_color::vertex_data()
{
    return _vertices.data();
}

_INLINE_ boole vertices_buffer_pos_color::write_binary(const string& path)
{
    if (!path.end_with(vertices_buffer::vertices_binary_extension_name))
    {
        return boole::False;
    }
    file f;
    if (!f.init_output(path.data(), boole::True))
    {
        return boole::False;
    }
    escape_function ef =
        [=]() mutable
        {
            f.uninit();
        };

    vertices_buffer_binary_header header;
    header.vertex_type = vertex_pos_color::type();
    header.vertex_number = vertex_count();
    header.index_number = vertex_index_count();

    if (!f.output(&header, sizeof(vertices_buffer_binary_header)))
    {
        return boole::False;
    }
    if (!f.output(_vertices.data(), _vertices.size() * sizeof(_vertices[0])))
    {
        return boole::False;
    }
    return boole::True;
}

_INLINE_ boole vertices_buffer_pos_color::setup(ref<json_base> json)
{
    _vertices.clear();

    json_cursor cursor(json);
    for (s64 i = 0; i < json->size(); ++i)
    {
        vertex_pos_color vertex =
        {
            {
                cursor[i]["position"][0].as_number().as_f32(),
                cursor[i]["position"][1].as_number().as_f32(),
                cursor[i]["position"][2].as_number().as_f32(),
            },
            {
                cursor[i]["color"][0].as_number().as_f32(),
                cursor[i]["color"][1].as_number().as_f32(),
                cursor[i]["color"][2].as_number().as_f32(),
            }
        };
        _vertices.push_back(vertex);
    }
    return boole::True;
}

_INLINE_ boole vertices_buffer_pos_color::setup(const vertices_buffer_binary_header& header, file input_f)
{
    _vertices.clear();
    _vertices.resize(header.vertex_number);

    s64 try_read = header.vertex_number * sizeof(vertex_pos_color);
    s64 len;
    input_f.input(_vertices.data(), try_read, len);
    if (try_read != len)
    {
        return boole::False;
    }
    return boole::True;
}

}
