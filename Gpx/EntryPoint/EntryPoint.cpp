
#include "../Gpx/Interface.hpp"

#include "VulkanWindowTest.hpp"
#include "VulkanPipelineTest.hpp"
#include "VulkanIndicesBufferTest.hpp"
#include "VulkanTransitionMatrixTest.hpp"

int main(int argc, char* argv[])
{
    vulkan_window_test();
    vulkan_pipeline_test();
    vulkan_indices_buffer_test();
    vulkan_transition_matrix_test();
    return 0;
}
