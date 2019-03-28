#include "window.h"

namespace snova {
bool window::init() {
	auto& instance = get();

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	instance.m_window_handle = glfwCreateWindow(800, 600, "supernova", nullptr, nullptr);

	if (!instance.m_window_handle) {
		FATAL_LOG("glfwCreateWindow returned null window handler!");
		return false;
	}

	glfwSetFramebufferSizeCallback(get().m_window_handle, window::on_framebuffer_resize);

	VERBOSE_LOG("Created window");
	return true;
}
bool window::should_close() { return glfwWindowShouldClose(get().m_window_handle); }

void window::destroy() {
	glfwDestroyWindow(get().m_window_handle);

	glfwTerminate();
}

glm::ivec2 window::get_window_size() {
	glm::ivec2 size;
	glfwGetFramebufferSize(get().m_window_handle, &size.x, &size.y);
	return size;
}

void window::register_resize_callback(resize_callback_t callback) { get().m_resize_callback = callback; }

void snova::window::on_framebuffer_resize(GLFWwindow*, int width, int height) {
	if (get().m_resize_callback) {
		get().m_resize_callback({width, height});
	}
}

}  // namespace snova
