
#include "../VerticesViewer.hpp"

namespace gpx
{

vertices_viewer::vertices_viewer() :
    _state((s64)vertices_viewer_state::Offline)
{
}

vertices_viewer_state vertices_viewer::state() const
{
    return (vertices_viewer_state)_state.get();
}

boole vertices_viewer::transfer_state(vertices_viewer_state from, vertices_viewer_state to)
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
