#ifndef __WINDOW_H
#define __WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "defines.h"

typedef struct {
	VkInstance instance;
	GLFWwindow *window;
} VulkanWindow;

SB_RESULT vkwnd_create(VulkanWindow *vkwnd, const char *appName, int major, int minor, int build, int width, int height);
void vkwnd_destroy(VulkanWindow *vkwnd);


#endif
