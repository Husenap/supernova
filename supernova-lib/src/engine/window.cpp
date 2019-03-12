#include "window.h"

#include "../precompiled.h"

#include <iostream>

namespace snova {
bool window::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window_handle = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::cout << extension_count << " extensions supported" << std::endl;

	return true;
}
bool window::should_close() {
	return glfwWindowShouldClose(m_window_handle);
}

void window::destroy() {
	glfwDestroyWindow(m_window_handle);

	glfwTerminate();
}

}  // namespace snova