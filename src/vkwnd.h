#ifndef __WINDOW_H
#define __WINDOW_H

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "defines.h"

typedef struct {
	VkInstance instance;
	GLFWwindow *window;
	VkPhysicalDevice phys_device;
	VkDevice log_device;
	VkQueue graphics_queue;
	VkSurfaceKHR surface;
} VulkanWindow;

SB_RESULT vkwnd_create(VulkanWindow *vkwnd, const char *appName, int major, int minor, int build, int width, int height);
void vkwnd_destroy(VulkanWindow *vkwnd);


#endif
