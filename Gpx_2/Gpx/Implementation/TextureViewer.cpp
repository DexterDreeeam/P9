
#include "../_Interface.hpp"

namespace gpx
{

texture_viewer::texture_viewer() :
    _state((s64)texture_viewer_state::Offline)
{
}

texture_viewer_state texture_viewer::state() const
{
    return (texture_viewer_state)_state.get();
}

boole texture_viewer::transfer_state(texture_viewer_state from, texture_viewer_state to)
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
