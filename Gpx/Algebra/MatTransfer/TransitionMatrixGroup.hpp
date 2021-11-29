#pragma once

namespace gpx
{

struct transition_matrix_group
{
    transition_matrix_group() :
        model_tm(),
        view_tm(),
        proj_tm()
    {
    }

    ~transition_matrix_group() = default;

    mat4x4  model_tm;
    mat4x4  view_tm;
    mat4x4  proj_tm;
};

}
