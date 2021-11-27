#pragma once

namespace gpx
{

struct vertices_buffer_binary_header
{
    gpx::vertex_type  vertex_type;
    s64               vertex_number;
    s64               index_number;
};

class vertices_buffer
{
public:
    const static string vertices_binary_extension_name;
    const static string vertices_json_extension_name;

public:
    vertices_buffer() = default;

    virtual ~vertices_buffer() = default;

    virtual gpx::vertex_type vertex_type() = 0;

    virtual s64 vertex_count() = 0;

    virtual s64 vertex_data_size() = 0;

    virtual s64 vertices_data_size() = 0;

    virtual void* vertex_data() = 0;

public:
    s64 vertex_index_count();

    vertex_index* vertex_index_data();

public:
    static ref<vertices_buffer> load(const string& path);

    virtual boole write_binary(const string& path) = 0;

private:
    static ref<vertices_buffer> load_from_json(const string& path);

    static ref<vertices_buffer> load_from_binary(const string& path);

private:
    vector<vertex_index>  _vertex_indices;
};

}
