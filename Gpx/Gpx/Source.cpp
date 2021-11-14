
#include "../../Sdk/Interface.hpp"

#include "../External/Vulkan/Windows/Header/vulkan/vulkan.h"
#include "../External/Vulkan/Windows/Header/glfw3.h"

void start_gpx()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(
        800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(
        nullptr, &extensionCount, nullptr);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
