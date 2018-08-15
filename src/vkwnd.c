#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "vkwnd.h"

VkApplicationInfo appInfo = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = NULL,
	.applicationVersion = 0,
	.pEngineName = "Snowball Engine",
	.engineVersion = VK_MAKE_VERSION(1, 0, 0),
	.apiVersion = VK_API_VERSION_1_1
};

VkInstanceCreateInfo createInfo = {
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pApplicationInfo = &appInfo,
	.enabledLayerCount = 0
};

SB_RESULT vkwnd_create(VulkanWindow *vkwnd, const char *appName, int major, int minor, int build, int width, int height) {
	uint32_t vkext = 0;
	uint32_t glfwextc;
	const char **glfwextv;
	VkResult result;

	glfwInit();

	// create a window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	vkwnd->window = glfwCreateWindow(width, height, appName, NULL, NULL);

	// print info about vulkan
	vkEnumerateInstanceExtensionProperties(NULL, &vkext, NULL);

	#ifdef DEBUG
		printf("%i Vulkan extensions supported\n", vkext);
	#endif

	// create a vulkan instance
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, build);

	glfwextv = glfwGetRequiredInstanceExtensions(&glfwextc);

	createInfo.enabledExtensionCount = glfwextc;
	createInfo.ppEnabledExtensionNames = glfwextv;

	result = vkCreateInstance(&createInfo, NULL, &vkwnd->instance);

	if (result != VK_SUCCESS) {
		printf("Unable to create vulkan instance. Error: %i\n", result);
		return SB_EXIT_FAILURE;
	}

	return SB_EXIT_SUCCESS;
}

void vkwnd_destroy(VulkanWindow *vkwnd) {
	vkDestroyInstance(vkwnd->instance, NULL);
	glfwDestroyWindow(vkwnd->window);
	glfwTerminate();
}
