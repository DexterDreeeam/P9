
#include "../../Interface.hpp"
#include "../GlfwWindow.hpp"

namespace gpx
{

ref<window> window::build(const window_desc& desc)
{
    AUTO_TRACE;

    switch (desc.type)
    {
    case window_desc::_type::Glfw:
        return ref<glfw_window>::new_instance(desc).ref_of<window>();

    case window_desc::_type::P9:
        return ref<window>();

    default:
        return ref<window>();
    }
}

}
