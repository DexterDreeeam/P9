#pragma once

void vulkan_uniform_buffer_test()
{
    using tsf = gpx::transform<gpx::Vulkan>;
    boole checker;

    // create runtime
    gpx::runtime_desc rt_desc;
    rt_desc.type = gpx::Vulkan;
    rt_desc.frame_count = 3;
    rt_desc.msaa_level = 0;
    rt_desc.debug_mode = boole::True;
    auto rt = gpx::runtime::build(rt_desc);
    checker = rt->init();
    assert(checker);

    // create window
    gpx::window_desc wnd_desc;
    wnd_desc.name = "Hello Pavilion Nine";
    wnd_desc.width = 240;
    wnd_desc.height = 180;
    auto wnd = rt->build_window(wnd_desc);
    checker = wnd->start();
    assert(checker);

    // build shader
    vector<string> vertex_shader_files_found;
    vector<string> fragment_shader_files_found;
    string vert_shader_file_name = "test3.vert.spv";
    string frag_shader_file_name = "test3.frag.spv";
    vertex_shader_files_found = search_files(
        "../",
        [&](const string& path)
        {
            return
                path.size() >= vert_shader_file_name.size() &&
                path.substr(path.size() - vert_shader_file_name.size()) == vert_shader_file_name;
        });
    fragment_shader_files_found = search_files(
        "../",
        [&](const string& path)
        {
            return
                path.size() >= frag_shader_file_name.size() &&
                path.substr(path.size() - frag_shader_file_name.size()) == frag_shader_file_name;
        });
    assert(vertex_shader_files_found.size());
    assert(fragment_shader_files_found.size());

    gpx::shader_desc shader_desc_1;
    shader_desc_1._type = gpx::shader_type::VERTEX;
    shader_desc_1._shader_name = "test vertex shader 1";
    shader_desc_1._shader_path = vertex_shader_files_found[0];
    auto vert_shader = rt->build_shader(shader_desc_1);
    checker = vert_shader.has_value();
    assert(checker);

    gpx::shader_desc shader_desc_2;
    shader_desc_2._type = gpx::shader_type::FRAGMENT;
    shader_desc_2._shader_name = "test fragment shader 1";
    shader_desc_2._shader_path = fragment_shader_files_found[0];
    auto frag_shader = rt->build_shader(shader_desc_2);
    checker = frag_shader.has_value();
    assert(checker);

    // register dynamic memory
    gpx::vec3 color_v3 = { 1.0, 1.0, 1.0 };
    gpx::dynamic_memory_desc dynamic_memory_desc1;
    dynamic_memory_desc1._name = "Pavilion Nine Test Dynamic Memory 1";
    dynamic_memory_desc1._size = sizeof(color_v3);

    checker = rt->register_dynamic_memory(dynamic_memory_desc1);
    assert(checker);

    tsf::MatTy transform_mat = tsf::MatTy::identity();
    gpx::dynamic_memory_desc dynamic_memory_desc2;
    dynamic_memory_desc2._name = "Pavilion Nine Test Dynamic Memory 2";
    dynamic_memory_desc2._size = sizeof(transform_mat);

    checker = rt->register_dynamic_memory(dynamic_memory_desc2);
    assert(checker);

    // register pipeline
    gpx::pipeline_desc pipeline_desc;
    pipeline_desc._window_name = "Hello Pavilion Nine";
    pipeline_desc._pipeline_name = "Pavilion Nine Test Pipeline";
    pipeline_desc._vertex_type = gpx::vertex_pos_color::type();
    pipeline_desc._shaders.push_back(vert_shader);
    pipeline_desc._shaders.push_back(frag_shader);
    pipeline_desc._dynamic_memories.push_back("Pavilion Nine Test Dynamic Memory 1");
    pipeline_desc._dynamic_memories.push_back("Pavilion Nine Test Dynamic Memory 2");
    pipeline_desc._texture_viewer_count = 0;

    checker = rt->register_pipeline(pipeline_desc);
    assert(checker);

    vert_shader->unload();
    frag_shader->unload();

    // vertices buffer
    string vertices_file_name_3 = "test3-1.p9vb";
    auto vertices_files_3 = search_files(
        "../",
        [&](const string& path)
        {
            return
                path.size() >= vertices_file_name_3.size() &&
                path.substr(path.size() - vertices_file_name_3.size()) == vertices_file_name_3;
        });
    assert(vertices_files_3.size());

    gpx::vertices_viewer_desc vv_desc;
    vv_desc._name = "test 3 vertices";
    vv_desc._file_path = vertices_files_3[0];
    checker = rt->register_vertices_viewer(vv_desc);
    assert(checker);

    checker = rt->load_vertices_viewer("test 3 vertices");
    assert(checker);

    vector<string> vv_names;
    vv_names.push_back("test 3 vertices");

    // load pipeline resource
    checker = rt->setup_pipeline_vertices_viewer("Pavilion Nine Test Pipeline", vv_names);
    assert(checker);

    checker = rt->load_pipeline_resource("Pavilion Nine Test Pipeline");
    assert(checker);

    // main loop
    u64 start_tick = tick::elapse();
    while (1)
    {
        if (tick::elapse() - start_tick >= 1000)
        {
            break;
        }
        checker = wnd->poll_event();
        assert(checker);

        s64 diff = tick::elapse() - start_tick;
        color_v3.x() = (diff / (f32)1000);
        color_v3.y() = (((diff + 333) % 1000) / (f32)1000);
        color_v3.z() = (((diff + 666) % 1000) / (f32)1000);
        checker = rt->update_dynamic_memory("Pavilion Nine Test Dynamic Memory 1", &color_v3);
        assert(checker);

        auto model = tsf::rotate_z((f32)diff / 1000 * math::pi());
        auto view = tsf::view(gpx::vec3(3, 0, 1), gpx::vec3(0, 0, 0), gpx::vec3(0, 0, 1));
        auto proj = tsf::perspective(math::pi() / 3, (f32)wnd_desc.width / wnd_desc.height, 0.1, 10);
        transform_mat = tsf::act(model, view, proj);

        checker = rt->update_dynamic_memory("Pavilion Nine Test Dynamic Memory 2", &transform_mat);
        assert(checker);

        checker = rt->render("Pavilion Nine Test Pipeline");
        assert(checker);
    }
    checker = rt->wait_render_complete();
    assert(checker);

    // release resources
    checker = rt->unload_pipeline_resource("Pavilion Nine Test Pipeline");
    assert(checker);
    checker = rt->unregister_dynamic_memory("Pavilion Nine Test Dynamic Memory 1");
    assert(checker);
    checker = rt->unregister_dynamic_memory("Pavilion Nine Test Dynamic Memory 2");
    assert(checker);
    checker = rt->unregister_pipeline("Pavilion Nine Test Pipeline");
    assert(checker);
    checker = rt->unload_vertices_viewer("test 3 vertices");
    assert(checker);
    checker = wnd->stop();
    assert(checker);
    checker = rt->uninit();
    assert(checker);
}
