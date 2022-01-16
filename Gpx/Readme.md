# Pavailion 9 Gpx Interface

> ## Runtime
> Runtime is core of Gpx, execute main logic flow for Graphics processing.
> `gpx::runtime` is base type of multi-implementation class.
>> ```cpp
>> * static ref<runtime> gpx::runtime::build(const gpx::runtime_desc& desc)
>> ```
>> `gpx::runtime::build` is a static function creating core runtime.
>> ```cpp
>> * vector<string> gpx::runtime::list_device()
>> ```
>> `gpx::runtime::list_device` get a list of usable physical device name.
>> ```cpp
>> * boole gpx::runtime::init(const string& preferred_device_name = "")
>> ```
>> `gpx::runtime::init` is help init runtime core,
>> input preferred_device_name running renderring job,
>> or let runtime auto select highest-performance device.
>> ```cpp
>> * boole gpx::runtime::uninit()
>> ```
>> `gpx::runtime::uninit` uninit a runtime which is already inited.
