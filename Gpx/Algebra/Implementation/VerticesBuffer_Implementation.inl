#pragma once

namespace gpx
{

_SELECTANY_ const string vertices_buffer::vertices_binary_extension_name = ".p9vb";
_SELECTANY_ const string vertices_buffer::vertices_json_extension_name = ".vertices.json";

_INLINE_ s64 vertices_buffer::vertex_index_count()
{
    return _vertex_indices.size();
}

_INLINE_ vertex_index* vertices_buffer::vertex_index_data()
{
    return _vertex_indices.data();
}

_INLINE_ ref<vertices_buffer> vertices_buffer::load(const string& path)
{
    if (path.end_with(vertices_binary_extension_name))
    {
        return load_from_binary(path);
    }
    else if (path.end_with(vertices_json_extension_name))
    {
        return load_from_json(path);
    }
    else
    {
        return ref<vertices_buffer>();
    }
}

_INLINE_ ref<vertices_buffer> vertices_buffer::load_from_json(const string& path)
{
    auto json = json_deserialize_from_file(path);
    if (json->type() != json_type::OBJECT ||
        json.ref_of<json_object>()->index("vertex_type").empty())
    {
        return ref<vertices_buffer>();
    }
    auto vertices_json = json->index("vertices");
    if (vertices_json.empty())
    {
        return ref<vertices_buffer>();
    }
    if (json.ref_of<json_object>()->index("vertex_type")->value() == "\"POS_COLOR\"")
    {
        auto vb = ref<vertices_buffer_pos_color>::new_instance();
        if (vb->setup(vertices_json))
        {
            return vb;
        }
    }
    return ref<vertices_buffer>();
}

_INLINE_ ref<vertices_buffer> vertices_buffer::load_from_binary(const string& path)
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
    vertices_buffer_binary_header header;
    f.input(&header, sizeof(header), len);
    if (len != sizeof(header))
    {
        assert(0);
        return ref<vertices_buffer>();
    }

    ref<vertices_buffer> rst;
    switch (header.vertex_type)
    {
    case vertex_type::POS_COLOR:
        rst = ref<vertices_buffer_pos_color>::new_instance();
        if (!rst.ref_of<vertices_buffer_pos_color>()->setup(header, f))
        {
            rst = ref<vertices_buffer>();
        }
        break;
    default:
        break;
    }
    return rst;
}

}
