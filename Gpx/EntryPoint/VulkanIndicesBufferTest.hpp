#pragma once

void vulkan_indices_buffer_test()
{
    boole checker;

    // create runtime
    gpx::runtime_desc rt_desc;
    rt_desc.type = gpx::Vulkan;
    rt_desc.frame_count = 3;
    rt_desc.msaa_level = 0;
    rt_desc.debug_mode = boole::False;// boole::True;
    auto rt = gpx::runtime::build(rt_desc);
    checker = rt->init();
    assert(checker);

    // create window
    gpx::window_desc wnd_desc;
    wnd_desc.name = "Hello Pavilion Nine";
    wnd_desc.width = 160;
    wnd_desc.height = 120;
    auto wnd = rt->build_window(wnd_desc);
    checker = wnd->start();
    assert(checker);

    // build shader
    vector<string> vertex_shader_files_found;
    vector<string> fragment_shader_files_found;
    string vert_shader_file_name = "test2.vert.spv";
    string frag_shader_file_name = "test2.frag.spv";
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
    shader_desc_1.type = gpx::shader_type::VERTEX;
    shader_desc_1.shader_name = "test vertex shader 1";
    shader_desc_1.shader_path = vertex_shader_files_found[0];
    auto vert_shader = rt->build_shader(shader_desc_1);
    checker = vert_shader.has_value();
    assert(checker);

    gpx::shader_desc shader_desc_2;
    shader_desc_2.type = gpx::shader_type::FRAGMENT;
    shader_desc_2.shader_name = "test fragment shader 1";
    shader_desc_2.shader_path = fragment_shader_files_found[0];
    auto frag_shader = rt->build_shader(shader_desc_2);
    checker = frag_shader.has_value();
    assert(checker);

    // register pipeline
    gpx::pipeline_desc pipeline_desc;
    pipeline_desc.window_name = "Hello Pavilion Nine";
    pipeline_desc.pipeline_name = "Pavilion Nine Test Pipeline";
    pipeline_desc.render_type = gpx::pipeline_render_type::basic;
    pipeline_desc.vertex_type = gpx::vertex_pos_color::type();
    pipeline_desc.shaders.push_back(vert_shader);
    pipeline_desc.shaders.push_back(frag_shader);
    pipeline_desc.dynamic_memory_count = 0;
    pipeline_desc.texture_viewer_count = 0;
    checker = rt->register_pipeline(pipeline_desc);
    assert(checker);

    vert_shader->unload();
    frag_shader->unload();

    // vertices buffer
    string vertices_file_name_3 = "test2-3.p9vb";
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

    vv_desc._name = "test2 vertices 3";
    vv_desc._file_path = vertices_files_3[0];
    checker = rt->register_vertices_viewer(vv_desc);
    assert(checker);

    checker = rt->load_vertices_viewer("test2 vertices 3");
    assert(checker);

    vector<string> vv_names;
    vv_names.push_back("test2 vertices 3");

    // load pipeline resource
    checker = rt->setup_pipeline_vertices_viewer("Pavilion Nine Test Pipeline", vv_names);
    assert(checker);

    checker = rt->load_pipeline_resource("Pavilion Nine Test Pipeline");
    assert(checker);

    // main loop
    u64 start_tick = tick::elapse();
    while (1)
    {
        if (tick::elapse() - start_tick >= 500)
        {
            break;
        }
        checker = wnd->poll_event();
        assert(checker);
        checker = rt->render("Pavilion Nine Test Pipeline");
        assert(checker);
    }
    checker = rt->wait_render_complete();
    assert(checker);

    checker = rt->unload_pipeline_resource("Pavilion Nine Test Pipeline");
    assert(checker);

    // release resources
    checker = rt->unregister_pipeline("Pavilion Nine Test Pipeline");
    assert(checker);
    checker = rt->unload_vertices_viewer("test2 vertices 3");
    assert(checker);
    checker = wnd->stop();
    assert(checker);
    checker = rt->uninit();
    assert(checker);
}
