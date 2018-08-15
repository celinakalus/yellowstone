#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "vkwnd.h"

int main(int argc, char **argv) {
	VulkanWindow vkwnd;

	vkwnd_create(&vkwnd, "vulkan_test", 1, 0, 0, 800, 600);

	// main event loop
	while (!glfwWindowShouldClose(vkwnd.window)) {
		glfwPollEvents();
	}

	vkwnd_destroy(&vkwnd);

	return 0;
}
