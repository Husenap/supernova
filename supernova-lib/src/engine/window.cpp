#include "window.h"

#include "../precompiled.h"

namespace snova {
bool window::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window_handle = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

	if (!m_window_handle) {
		FATAL_LOG("glfwCreateWindow returned null window handler!");
		return false;
	}

	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	INFO_LOG("%d extensions supported.", extension_count);

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