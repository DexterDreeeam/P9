#pragma once

#include "../../Algebra/Interface.hpp"

namespace gpx
{

class vertices_buffer
{
public:
    vertices_buffer() = default;

    virtual ~vertices_buffer() = default;

    virtual boole setup(const string& path) = 0;

    virtual gpx::vertex_type vertex_type() = 0;

    virtual s64 vertex_count() = 0;

    virtual s64 vertex_data_size() = 0;

    virtual s64 vertices_data_size() = 0;

    virtual void* vertex_data() = 0;

public:
    s64 vertex_index_count();

    vertex_index* vertex_index_data();

private:
    vector<vertex_index>  _vertex_indices;
};

class vertices_buffer_pos_color : public vertices_buffer
{
public:
    vertices_buffer_pos_color() = default;

    virtual ~vertices_buffer_pos_color() override = default;

    virtual boole setup(const string& path) override;

    virtual gpx::vertex_type vertex_type() override;

    virtual s64 vertex_count() override;

    virtual s64 vertex_data_size() override;

    virtual s64 vertices_data_size() override;

    virtual void* vertex_data() override;

private:
    vector<vertex_pos_color>  _vertices;
};

}
