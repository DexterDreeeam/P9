Pavailion 9 Gpx Interface
=========

## Runtime
Runtime is core of Gpx, execute main logic flow for Graphics processing.<br>
`gpx::runtime` is base type of multi-implementation class.

---------

* ### Runtime Basic Interface

```cpp
static ref<gpx::runtime::runtime> gpx::runtime::build(const gpx::runtime_desc& desc)
```
build a core runtime and get reference to it.

```cpp
vector<string> gpx::runtime::list_device()
```
get a list of usable physical device name.

```cpp
boole gpx::runtime::init(const string& preferred_device_name = "")
```
init a runtime core which is built already,
input preferred_device_name running renderring job,
or let runtime auto select highest-performance device.

```cpp
boole gpx::runtime::uninit()
```
uninit a runtime which is already inited.

* ### Runtime Window Interface

```cpp
ref<gpx::window> gpx::runtime::build_window(const gpx::window_desc& desc)
```
build a window and get reference to it.

```cpp
boole gpx::runtime::remove_window(const string& window_name)
```
remove a window by window name.

```cpp
ref<gpx::window> gpx::runtime::get_window(const string& window_name)
```
get a window reference by window name.

* ### Runtime Shader Interface

```cpp
ref<gpx::shader> gpx::runtime::build_shader(const gpx::shader_desc& desc)
```
build a shader and get reference to it.

* ### Runtime Dynamic Memory Interface

```cpp
boole register_dynamic_memory(const gpx::dynamic_memory_desc& desc)
```
build a dynamic memory and get reference to it.

```cpp
boole unregister_dynamic_memory(const string& dynamic_memory)
```
destroy a registered dynamic memory which is not used anymore.

```cpp
boole update_dynamic_memory(const string& dynamic_memory, void* src)
```
update a dynamic memory content, input a pointer of source buffer.

```cpp
boole setup_pipeline_dynamic_memory(const string& pipeline, const vector<string>& dm_vec)
```
setup dynamic memory list for specific pipeline.


