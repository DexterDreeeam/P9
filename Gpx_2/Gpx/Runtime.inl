#pragma once

namespace gpx
{

    ref<runtime> runtime::build(const runtime_desc& desc)
    {
        AUTO_TRACE;

        ref<runtime> ret;

        switch (desc.type)
        {
        case Vulkan:
            ret = ref<vulkan_runtime>::new_instance(desc);
            break;

        default:
            assert(0);
            break;
        }

        return ret;
    }

}
