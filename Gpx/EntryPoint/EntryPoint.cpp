
#include "../Gpx/Interface.hpp"

#include "VulkanWindowTest.hpp"
#include "VulkanPipelineTest.hpp"
#include "VulkanIndicesBufferTest.hpp"
#include "VulkanUniformBufferTest.hpp"

using tt = gpx::transform<gpx::algebra_type::Vulkan>;

int main(int argc, char* argv[])
{
    //vulkan_window_test();
    //vulkan_pipeline_test();
    //vulkan_indices_buffer_test();
    //vulkan_uniform_buffer_test();

    auto vm = tt::view(gpx::vec3(0,0,1), gpx::vec3(1,0.1,0), gpx::vec3(0, 1, 0));
    auto test = tt::act(gpx::vec4(0, 0, 0, 1), vm);

    return 0;
}
