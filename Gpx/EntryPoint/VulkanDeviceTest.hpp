#pragma once

void vulkan_device_test()
{
    boole checker;

    gpx::runtime_desc rt_desc;
    rt_desc.type = gpx::runtime_desc::_type::Vulkan;
    rt_desc.debug_mode = boole::True;

    auto rt = gpx::runtime::build(rt_desc);
    checker = rt->init();
    assert(checker);

    auto vec_device = rt->list_device();
    assert(vec_device.size() > 0);
    for (auto& device_name : vec_device)
    {
        print("Vulkan Find Physical Device: %s\n", device_name.data());
    }

    checker = rt->select_device(vec_device[0]);
    assert(checker);

    checker = rt->uninit();
    assert(checker);
}
