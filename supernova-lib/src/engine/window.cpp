#include "window.h"

#include "../precompiled.h"

namespace snova {
bool window::init() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window_handle = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

	if (!m_window_handle) {
		FATAL_LOG("glfwCreateWindow returned null window handler!");
		return false;
	}

	VERBOSE_LOG("Created window");
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