#include "defines.h"
#include <stdint.h>
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

	for (int device_i = 0; device_i < (long)device_count; device_i++) {
		VkPhysicalDevice device = phys_devices[device_i];
		VkPhysicalDeviceProperties device_prop;
		vkGetPhysicalDeviceProperties(device, &device_prop);
		printf("  [%i] %s\n", device_i, device_prop.deviceName);
	}

	printf("Choosing first device.\n");

	vkwnd->phys_device = phys_devices[0];
	free(phys_devices);
	(void)phys_devices;

	VkQueueFamilyProperties *queue_prop;
	uint32_t queue_count = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(vkwnd->phys_device, &queue_count, NULL);

	queue_prop = malloc(sizeof(VkQueueFamilyProperties) * queue_count);
	vkGetPhysicalDeviceQueueFamilyProperties(vkwnd->phys_device, &queue_count, queue_prop);

	int queue_chosen = -1;
	for (int i = 0; i < (long)queue_count; i++) {
		if (queue_prop[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			queue_chosen = i;
			break;
		}
	}

	if (queue_chosen < 0) {
		printf("No suitable queue found.\n");
		return SB_EXIT_FAILURE;
	}

	printf("Graphics queue %i chosen.\n", queue_chosen);

	VkDeviceQueueCreateInfo queue_create_info = { 0, };
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = queue_chosen;
	queue_create_info.queueCount = 1;

	float queue_priority = 1.0f;
	queue_create_info.pQueuePriorities = &queue_priority;

	VkPhysicalDeviceFeatures device_features = { 0, };

	VkDeviceCreateInfo log_dev_create_info = { 0, };
	log_dev_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	log_dev_create_info.pQueueCreateInfos = &queue_create_info;
	log_dev_create_info.queueCreateInfoCount = 1;
	log_dev_create_info.pEnabledFeatures = &device_features;
	log_dev_create_info.enabledExtensionCount = 0;
	log_dev_create_info.enabledLayerCount = 0;

	result = vkCreateDevice(vkwnd->phys_device, &log_dev_create_info, NULL, &(vkwnd->log_device));

	if (result != VK_SUCCESS) {
		printf("Failed to create logical device: %i\n", result);
	}

	printf("Vulkan window created successfully.\n");

	vkGetDeviceQueue(vkwnd->log_device, queue_chosen, 0, &(vkwnd->graphics_queue));

	return SB_EXIT_SUCCESS;
}

void vkwnd_destroy(VulkanWindow *vkwnd) {
	vkDestroyDevice(vkwnd->log_device, NULL);
	vkDestroyInstance(vkwnd->instance, NULL);
	glfwDestroyWindow(vkwnd->window);
	glfwTerminate();
}
