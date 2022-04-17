
#include "../Runtime.hpp"
#include "../Vulkan/VulkanRuntime.hpp"

namespace gpx
{

ref<runtime> runtime::build(const runtime_desc& desc)
{
    AUTO_TRACE;

    ref<runtime> ret;

    switch (desc.type)
    {
    case Vulkan:
        ret = vulkan_runtime::build(desc).ref_of<runtime>();
        break;

    default:
        ret = ref<runtime>();
        break;
    }

    return ret;
}

}
