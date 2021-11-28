#pragma once

namespace gpx
{

struct vertices_buffer_header
{
    gpx::vertex_type  vertex_type;
    s64               vertices_count;
    s64               vertices_data_size;
    s64               indices_count;
    s64               indices_data_size;
};

class vertices_buffer
{
public:
    const static string vertices_binary_extension_name;
    const static string vertices_json_extension_name;

public:
    vertices_buffer();

    ~vertices_buffer();

    vertex_type vertices_type() const;

public:
    const void* data() const;

public:
    s64 vertices_count() const;

    s64 vertices_data_size() const;

    s64 vertices_offset() const;

public:
    s64 vertex_indices_count() const;

    s64 vertex_indices_data_size() const;

    s64 vertex_indices_offset() const;

public:
    // fn_alloc provide a function, which input sz_t, output void* as memory allocation address
    template<typename Alloc_Fn_Ty>
    static ref<vertices_buffer> load_with_allocator(const string& path, Alloc_Fn_Ty fn_alloc);

    static ref<vertices_buffer> load(const string& path);

    boole write_binary(const string& path) const;

private:
    template<typename Alloc_Fn_Ty>
    static ref<vertices_buffer> load_from_json(const string& path, Alloc_Fn_Ty fn_alloc);

    template<typename Alloc_Fn_Ty>
    static ref<vertices_buffer> load_from_binary(const string& path, Alloc_Fn_Ty fn_alloc);

private:
    template<typename Alloc_Fn_Ty>
    boole setup_vertex_pos_color(ref<json_base> json, Alloc_Fn_Ty fn_alloc);

protected:
    void*         _data;
    void*         _internal_data;
    vertex_type   _type;

    s64           _vertices_count;
    s64           _vertices_data_size;
    s64           _vertices_offset;

    s64           _vertex_idx_count;
    s64           _vertex_idx_data_size;
    s64           _vertex_idx_offset;
};

}
