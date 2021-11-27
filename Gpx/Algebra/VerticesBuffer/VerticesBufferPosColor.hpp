#pragma once

namespace gpx
{

class vertices_buffer_pos_color : public vertices_buffer
{
public:
    vertices_buffer_pos_color() = default;

    virtual ~vertices_buffer_pos_color() override = default;

    virtual gpx::vertex_type vertex_type() override;

    virtual s64 vertex_count() override;

    virtual s64 vertex_data_size() override;

    virtual s64 vertices_data_size() override;

    virtual void* vertex_data() override;

    virtual boole write_binary(const string& path) override;

public:
    boole setup(ref<json_base> json);

    boole setup(const vertices_buffer_binary_header& header, file input_f);

private:
    vector<vertex_pos_color>  _vertices;
};

}
