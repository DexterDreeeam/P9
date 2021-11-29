#pragma once

namespace gpx
{

_SELECTANY_ const string vertices_buffer::vertices_binary_extension_name = ".p9vb";
_SELECTANY_ const string vertices_buffer::vertices_json_extension_name = ".vertices.json";

_INLINE_ vertices_buffer::vertices_buffer() :
    _data(nullptr),
    _internal_data(nullptr),
    _vertices_count(-1),
    _vertices_data_size(-1),
    _vertices_offset(-1),
    _vertex_idx_count(-1),
    _vertex_idx_data_size(-1),
    _vertex_idx_offset(-1)
{
}

_INLINE_ vertices_buffer::~vertices_buffer()
{
    if (_internal_data)
    {
        memory::free(_internal_data);
        _internal_data = nullptr;
    }
}

_INLINE_ vertex_type vertices_buffer::vertices_type() const
{
    return _type;
}

_INLINE_ const void* vertices_buffer::data() const
{
    return _data;
}

_INLINE_ s64 vertices_buffer::vertices_count() const
{
    return _vertices_count;
}

_INLINE_ s64 vertices_buffer::vertices_data_size() const
{
    return _vertices_data_size;
}

_INLINE_ s64 vertices_buffer::vertices_offset() const
{
    return _vertices_offset;
}

_INLINE_ s64 vertices_buffer::vertex_indices_count() const
{
    return _vertex_idx_count;
}

_INLINE_ s64 vertices_buffer::vertex_indices_data_size() const
{
    return _vertex_idx_data_size;
}

_INLINE_ s64 vertices_buffer::vertex_indices_offset() const
{
    return _vertex_idx_offset;
}

template<typename Alloc_Fn_Ty>
static ref<vertices_buffer> vertices_buffer::load_with_allocator(const string& path, Alloc_Fn_Ty fn_alloc)
{
    if (path.end_with(vertices_binary_extension_name))
    {
        return load_from_binary(path, fn_alloc);
    }
    else if (path.end_with(vertices_json_extension_name))
    {
        return load_from_json(path, fn_alloc);
    }
    else
    {
        return ref<vertices_buffer>();
    }
}

_INLINE_ ref<vertices_buffer> vertices_buffer::load(const string& path)
{
    auto rst = ref<vertices_buffer>();
    auto alloc_fn = [](s64 sz) { return memory::alloc(sz); };
    if (path.end_with(vertices_binary_extension_name))
    {
        rst = load_from_binary(path, alloc_fn);
    }
    else if (path.end_with(vertices_json_extension_name))
    {
        rst = load_from_json(path, alloc_fn);
    }
    else
    {
        return ref<vertices_buffer>();
    }
    rst->_internal_data = rst->_data;
    return rst;
}

_INLINE_ boole vertices_buffer::write_binary(const string& path) const
{
    if (!path.end_with(vertices_buffer::vertices_binary_extension_name))
    {
        return boole::False;
    }
    if (!_data)
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

    vertices_buffer_header header;
    header.vertex_type = vertices_type();
    header.vertices_count = vertices_count();
    header.vertices_data_size = vertices_data_size();
    header.indices_count = vertex_indices_count();
    header.indices_data_size = vertex_indices_data_size();

    if (!f.output(&header, sizeof(vertices_buffer_header)))
    {
        return boole::False;
    }
    if (!f.output(_data, vertices_data_size() + vertex_indices_data_size()))
    {
        return boole::False;
    }
    return boole::True;
}

template<typename Alloc_Fn_Ty>
_INLINE_ ref<vertices_buffer> vertices_buffer::load_from_json(const string& path, Alloc_Fn_Ty fn_alloc)
{
    auto json = json_deserialize_from_file(path);
    if (json->type() != json_type::OBJECT ||
        json.ref_of<json_object>()->index("vertex_type").empty())
    {
        return ref<vertices_buffer>();
    }
    auto vertices_json = json->index("vertices");
    s64 vertices_space = 0;
    if (vertices_json.empty())
    {
        return ref<vertices_buffer>();
    }
    auto indices_json = json->index("indices");
    s64 indices_count = 0;
    s64 indices_space = 0;
    if (indices_json.has_value())
    {
        indices_count = indices_json->size();
        indices_space = indices_count * sizeof(vertex_index);
    }
    auto rst = ref<vertices_buffer>::new_instance();
    if (json.ref_of<json_object>()->index("vertex_type")->value() == "\"POS_COLOR\"")
    {
        vertices_space = rst->setup_vertex_pos_color(vertices_json, fn_alloc, indices_space);
        if (vertices_space <= 0)
        {
            return ref<vertices_buffer>();
        }
    }
    if (indices_json.has_value())
    {
        auto cursor = json_cursor(indices_json);
        for (s64 i = 0; i < indices_count; ++i)
        {
            vertex_index* p = pointer_convert(rst->_data, vertices_space + i * sizeof(vertex_index), vertex_index*);
            p->_idx = cursor[i].as_number().as_f32();
        }
    }
    rst->_vertex_idx_count = indices_count;
    rst->_vertex_idx_data_size = indices_space;
    rst->_vertex_idx_offset = vertices_space;
    return rst;
}

template<typename Alloc_Fn_Ty>
_INLINE_ ref<vertices_buffer> vertices_buffer::load_from_binary(const string& path, Alloc_Fn_Ty fn_alloc)
{
    file f;
    if (!f.init_input(path.data()))
    {
        return ref<vertices_buffer>();
    }
    escape_function ef =
        [=]() mutable
        {
            f.uninit();
        };

    s64 len;
    vertices_buffer_header header;
    f.input(&header, sizeof(header), len);
    if (len != sizeof(header))
    {
        assert(0);
        return ref<vertices_buffer>();
    }
    auto rst = ref<vertices_buffer>::new_instance();
    s64 space = header.vertices_data_size + header.indices_data_size;
    rst->_data = fn_alloc(space);
    if (!rst->_data)
    {
        assert(0);
        return ref<vertices_buffer>();
    }
    f.input(rst->_data, space, len);
    if (len != space)
    {
        assert(0);
        return ref<vertices_buffer>();
    }
    rst->_type = header.vertex_type;
    rst->_vertices_count = header.vertices_count;
    rst->_vertices_data_size = header.vertices_data_size;
    rst->_vertices_offset = 0;
    rst->_vertex_idx_count = header.indices_count;
    rst->_vertex_idx_data_size = header.indices_data_size;
    rst->_vertex_idx_offset = header.vertices_data_size;
    return rst;
}

template<typename Alloc_Fn_Ty>
_INLINE_ s64 vertices_buffer::setup_vertex_pos_color(ref<json_base> json, Alloc_Fn_Ty fn_alloc, s64 indices_space)
{
    _vertices_count = json->size();
    _vertices_data_size = _vertices_count * sizeof(vertex_pos_color);
    _vertices_offset = 0;
    _data = fn_alloc(_vertices_data_size + indices_space);
    if (!_data)
    {
        return boole::False;
    }
    json_cursor cursor(json);
    for (s64 i = 0; i < _vertices_count; ++i)
    {
        auto* p = pointer_convert(_data, i * sizeof(vertex_pos_color), vertex_pos_color*);
        p->_pos.x() = cursor[i]["position"][0].as_number().as_f32();
        p->_pos.y() = cursor[i]["position"][1].as_number().as_f32();
        p->_pos.z() = cursor[i]["position"][2].as_number().as_f32();
        p->_color.x() = cursor[i]["color"][0].as_number().as_f32();
        p->_color.y() = cursor[i]["color"][1].as_number().as_f32();
        p->_color.z() = cursor[i]["color"][2].as_number().as_f32();
    }
    return boole::True;
}

}
