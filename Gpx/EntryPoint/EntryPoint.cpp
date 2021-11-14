
#include "../Gpx/Interface.hpp"

int main(int argc, char* argv[])
{
    boole checker;

    gpx::runtime_desc rt_desc;
    rt_desc.type = gpx::runtime_desc::_type::Vulkan;
    rt_desc.debug_mode = boole::True;

    auto rt = gpx::runtime::build(rt_desc);
    checker = rt->init();
    assert(checker);

    gpx::window_desc wnd_desc;
    wnd_desc.type = gpx::window_desc::_type::Glfw;
    wnd_desc.name = "hello, Pavilion Nine";
    wnd_desc.width = 1920;
    wnd_desc.height = 1080;

    auto wnd = gpx::window::build(wnd_desc);
    checker = wnd->start();
    assert(checker);

    tick::sleep(2000);

    checker = wnd->stop();
    assert(checker);
    checker = rt->uninit();
    assert(checker);

    return 0;
}
