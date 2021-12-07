
#include "../Interface.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

ref<runtime> runtime::build(const runtime_desc& desc)
{
    AUTO_TRACE;

    ref<runtime> ret;

    switch (desc.type)
    {
    case Vulkan:
        ret = ref<vulkan_runtime>::new_instance(desc).ref_of<runtime>();
        break;

    default:
        ret = ref<runtime>();
        break;
    }

    ret->setup_self(ret);
    return ret;
}

}
