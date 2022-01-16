**Pavailion 9 Gpx Interface**
=========

## **Runtime**
Runtime is core of Gpx, execute main logic flow for Graphics processing.<br>
`gpx::runtime` is base type of multi-implementation class.

---------

* ### Runtime Basic Interface

> ```cpp
> static ref<gpx::runtime::runtime> gpx::runtime::build(const gpx::runtime_desc& desc)
> ```
> <small>*build a core runtime and get reference to it.*</small>

> ```cpp
> vector<string> gpx::runtime::list_device()
> ```
> <small>*get a list of usable physical device name.*</small>

> ```cpp
> boole gpx::runtime::init(const string& preferred_device_name = "")
> ```
> <small>*init a runtime core which is built already,
> input preferred_device_name running renderring job,
> or let runtime auto select highest-performance device.*</small>

> ```cpp
> boole gpx::runtime::uninit()
> ```
> <small>*uninit a runtime which is already inited.*</small>

* ### Runtime Window Interface

> ```cpp
> ref<gpx::window> gpx::runtime::build_window(const gpx::window_desc& desc)
> ```
> <small>*build a window and get reference to it.*</small>

> ```cpp
> boole gpx::runtime::remove_window(const string& window_name)
> ```
> <small>*remove a window by window name.*</small>

> ```cpp
> ref<gpx::window> gpx::runtime::get_window(const string& window_name)
> ```
> <small>*get a window reference by window name.*</small>

* ### Runtime Shader Interface

> ```cpp
> ref<gpx::shader> gpx::runtime::build_shader(const gpx::shader_desc& desc)
> ```
> <small>*build a shader and get reference to it.*</small>

* ### Runtime Dynamic Memory Interface

> ```cpp
> boole gpx::runtime::register_dynamic_memory(const gpx::dynamic_memory_desc& desc)
> ```
> <small>*build a dynamic memory and get reference to it.*</small>

> ```cpp
> boole gpx::runtime::unregister_dynamic_memory(const string& dynamic_memory)
> ```
> <small>*destroy a registered dynamic memory which is not used anymore.*</small>

> ```cpp
> boole gpx::runtime::update_dynamic_memory(const string& dynamic_memory, void* src)
> ```
> <small>*update a dynamic memory content, input a pointer of source buffer.*</small>

> ```cpp
> boole gpx::runtime::setup_pipeline_dynamic_memory(const string& pipeline, const vector<string>& dm_vec)
> ```
> <small>*setup dynamic memory list for specific pipeline.*</small>

* ### Runtime Texture Viewer Interface

> ```cpp
> boole gpx::runtime::register_texture_viewer(const gpx::texture_viewer_desc& desc)
> ```
> <small>*build a texture viewer and get reference to it.*</small>

> ```cpp
> boole gpx::runtime::unregister_texture_viewer(const string& texture_viewer)
> ```
> <small>*destroy a texture viewer.*</small>

> ```cpp
> boole gpx::runtime::load_texture_viewer(const string& texture_viewer)
> ```
> <small>*load texture resource to Gpu memory.*</small>

> ```cpp
> boole gpx::runtime::unload_texture_viewer(const string& texture_viewer)
> ```
> <small>*unload texture resource from Gpu memory.*</small>

> ```cpp
> boole gpx::runtime::setup_pipeline_texture_viewer(const string& pipeline_name, const vector<string>& viewers)
> ```
> <small>*setup texture viewer list for specific pipeline, these texture viewer are loaded already*</small>

* ### Runtime Vertices Viewer Interface

> ```cpp
> boole gpx::runtime::register_vertices_viewer(const gpx::vertices_viewer_desc& desc)
> ```
> <small>*build a vertices viewer and get reference to it.*</small>

> ```cpp
> boole gpx::runtime::unregister_vertices_viewer(const string& vertices_viewer)
> ```
> <small>*destroy a vertices viewer.*</small>

> ```cpp
> boole gpx::runtime::load_vertices_viewer(const string& vertices_viewer)
> ```
> <small>*load vertices resource to Gpu memory.*</small>

> ```cpp
> boole gpx::runtime::unload_vertices_viewer(const string& vertices_viewer)
> ```
> <small>*unload vertices resource from Gpu memory.*</small>

> ```cpp
> boole gpx::runtime::setup_pipeline_vertices_viewer(const string& pipeline_name, const vector<string>& viewers)
> ```
> <small>*setup vertices viewer list for specific pipeline, these vertices viewer are loaded already*</small>

* ### Runtime Pipeline Interface

> ```cpp
> boole gpx::runtime::register_pipeline(const gpx::pipeline_desc& desc)
> ```
> <small>*build a pipeline.*</small>

> ```cpp
> boole gpx::runtime::unregister_pipeline(const string& pipeline_name)
> ```
> <small>*destroy a pipeline.*</small>

> ```cpp
> boole gpx::runtime::load_pipeline_resource(const string& pipeline_name)
> ```
> <small>*load all pipeline resources,
> `setup_pipeline_dynamic_memory`, `setup_pipeline_texture_viewer`, `setup_pipeline_vertices_viewer`,
> are supposed to setup this pipeline before this function is called.*</small>

> ```cpp
> boole gpx::runtime::unload_pipeline_resource(const string& pipeline_name)
> ```
> <small>*unload pipeline resources, setup new resource for this pipeline or load pipeline in the future.*</small>

> ```cpp
> boole gpx::runtime::render(const string& pipeline_name)
> ```
> <small>*do one time renderring process to window.*</small>

> ```cpp
> boole gpx::runtime::wait_render_complete()
> ```
> <small>*wait all pipelines complete renderring.*</small>
