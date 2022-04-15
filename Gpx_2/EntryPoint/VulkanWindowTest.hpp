#pragma once

void vulkan_window_test()
{
    boole checker;

    gpx::runtime_desc rt_desc;
    rt_desc.type = gpx::Vulkan;
    rt_desc.frame_count = 3;
    rt_desc.msaa_level = 0;
    rt_desc.debug_mode = boole::False;

    auto rt = gpx::runtime::build(rt_desc);
    checker = rt->init();
    assert(checker);

    gpx::window_desc wnd_desc1;
    wnd_desc1.name = "Hello Pavilion Nine 1";
    wnd_desc1.width = 1920;
    wnd_desc1.height = 1080;

    gpx::window_desc wnd_desc2;
    wnd_desc2.name = "Hello Pavilion Nine 2";
    wnd_desc2.width = 720;
    wnd_desc2.height = 640;

    gpx::window_desc wnd_desc3;
    wnd_desc2.name = "Hello Pavilion Nine 3";
    wnd_desc2.width = 360;
    wnd_desc2.height = 480;

    auto wnd1 = rt->build_window(wnd_desc1);
    auto wnd2 = rt->build_window(wnd_desc2);
    auto wnd3 = rt->build_window(wnd_desc3);

    checker = wnd1->start();
    assert(checker);

    tick::sleep(100);

    checker = wnd2->start();
    assert(checker);

    tick::sleep(100);

    checker = wnd3->start();
    assert(checker);

    tick::sleep(100);

    checker = wnd1->stop();
    assert(checker);

    tick::sleep(100);

    checker = wnd1->start();
    assert(checker);

    tick::sleep(100);

    checker = wnd1->stop();
    assert(checker);
    checker = wnd2->stop();
    assert(checker);
    checker = wnd3->stop();
    assert(checker);

    checker = rt->uninit();
    assert(checker);
}
