#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	vkwnd->window = glfwCreateWindow(width, height, appName, NULL, NULL);

	if (vkwnd->window == NULL) {
		printf("Window creation failed!\n");
		return SB_EXIT_FAILURE;
	}

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

	/* pick physical device */
	uint32_t device_count = 0;
	VkPhysicalDevice *phys_devices = NULL;

	result = vkEnumeratePhysicalDevices(vkwnd->instance, &device_count, NULL);

	if (result < 0) {
		printf("Error while enumerating physical devices: %i\n", result);
		return SB_EXIT_FAILURE;
	}

	if (device_count <= 0) {
		printf("No physical devices found; Aborting.\n");
		return SB_EXIT_FAILURE;
	}

	printf("Found %u physical devices:\n", device_count);

	phys_devices = malloc(sizeof(VkPhysicalDevice) * device_count);

	if (phys_devices == NULL) {
		printf("Out of memory.\n");
		return SB_EXIT_FAILURE;
	}

	result = vkEnumeratePhysicalDevices(vkwnd->instance, &device_count, phys_devices);

	if (result < 0) {
		printf("Error while enumerating physical devices: %i\n", result);
		return SB_EXIT_FAILURE;
	}

	for (int device_i = 0; device_i < device_count; device_i++) {
		VkPhysicalDevice device = phys_devices[device_i];
		VkPhysicalDeviceProperties device_prop;
		vkGetPhysicalDeviceProperties(device, &device_prop);
		printf("  [%i] %s\n", device_i, device_prop.deviceName);
	}

	printf("Choosing first device.\n");

	VkPhysicalDevice phys_device = phys_devices[0];
	free(phys_devices);
	(void)phys_devices;

	printf("Vulkan window created successfully.\n");

	return SB_EXIT_SUCCESS;
}

void vkwnd_destroy(VulkanWindow *vkwnd) {
	vkDestroyInstance(vkwnd->instance, NULL);
	glfwDestroyWindow(vkwnd->window);
	glfwTerminate();
}
