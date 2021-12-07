
#include "../Interface.hpp"

namespace gpx
{

pipeline::pipeline() :
    _state((s64)pipeline_state::Uninit)
{
}

pipeline_state pipeline::state() const
{
    return (pipeline_state)_state.get();
}

boole pipeline::transfer_state(pipeline_state from, pipeline_state to)
{
    s64 f = (s64)from;
    s64 t = (s64)to;
    if (_state.compare_exchange(f, t) == f)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

}
