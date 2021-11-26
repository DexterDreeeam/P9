
#include "../PipelineBuffer.hpp"

namespace gpx
{

s64 vertices_buffer::vertex_index_count()
{
    return _vertex_indices.size();;
}

vertex_index* vertices_buffer::vertex_index_data()
{
    return _vertex_indices.data();
}

boole vertices_buffer_pos_color::setup(const string& path)
{
    _vertices.push_back(
        vertex_pos_color({{ 0.0f, -0.5f, 0.5f }, {0.8f, 0.0f, 0.0f}})
    );

    _vertices.push_back(
        vertex_pos_color({{ 0.5f, 0.5f, 0.5f }, {0.0f, 0.8f, 0.0f}})
    );

    _vertices.push_back(
        vertex_pos_color({{ -0.5f, 0.5f, 0.5f }, {0.0f, 0.0f, 0.8f}})
    );

    return boole::True;
}

vertex_type vertices_buffer_pos_color::vertex_type()
{
    return vertex_pos_color::type();
}

s64 vertices_buffer_pos_color::vertex_count()
{
    return _vertices.size();
}

s64 vertices_buffer_pos_color::vertex_data_size()
{
    return sizeof(vertex_pos_color);
}

s64 vertices_buffer_pos_color::vertices_data_size()
{
    return vertex_data_size() * vertex_count();
}

void* vertices_buffer_pos_color::vertex_data()
{
    return _vertices.data();
}

}
