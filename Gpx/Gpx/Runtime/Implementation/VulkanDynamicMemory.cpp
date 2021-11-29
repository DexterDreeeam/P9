
#include "../VulkanDynamicMemory.hpp"
#include "../VulkanRuntime.hpp"

namespace gpx
{

vulkan_dynamic_memory::vulkan_dynamic_memory(obs<vulkan_runtime> rt) :
    dynamic_memory(),
    _desc(),
    _rt(rt)
{
    AUTO_TRACE;
}

vulkan_dynamic_memory::~vulkan_dynamic_memory()
{
    AUTO_TRACE;

    uninit();
}

boole vulkan_dynamic_memory::init(const dynamic_memory_desc& desc)
{
    AUTO_TRACE;

    auto rt = _rt.try_ref();
    if (rt.empty())
    {
        return boole::False;
    }



    _desc = desc;
    return boole::True;
}

boole vulkan_dynamic_memory::uninit()
{
    return boole::True;
}

boole vulkan_dynamic_memory::update(void* src)
{
    return boole::True;
}

}
