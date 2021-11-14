
#include "../../Interface.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

ref<runtime> runtime::build(const runtime_desc& desc)
{
    AUTO_TRACE;

    switch (desc.type)
    {
    case runtime_desc::_type::Vulkan:
        return ref<vulkan_runtime>::new_instance(desc).ref_of<runtime>();

    default:
        return ref<runtime>();
    }
}

}
