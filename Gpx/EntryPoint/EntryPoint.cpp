
#include "../Gpx/Interface.hpp"

#include "VulkanWindowTest.hpp"
#include "VulkanPipelineTest.hpp"
#include "VulkanIndicesBufferTest.hpp"
#include "VulkanUniformBufferTest.hpp"

int main(int argc, char* argv[])
{
    vulkan_window_test();
    vulkan_pipeline_test();
    vulkan_indices_buffer_test();
    vulkan_uniform_buffer_test();

    auto m = gpx::transform<gpx::algebra_type::Vulkan>::rotate_x(math::pi() / 3);
    auto ep = math::epsilon() * 100000;

    f64 t1 = 1.0 / 3.0;
    f64 t2 = (10.0 / 3.0 + 1.0) / 13.000001;

    auto rst = math::equal(t1, t2);

    return 0;
}
