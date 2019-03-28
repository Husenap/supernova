#pragma once

typedef struct GLFWwindow GLFWwindow;

#include "../precompiled.h"

namespace snova {

using resize_callback_t = std::function<void(glm::ivec2)>;

class window : public singleton<window> {
public:
	window() {}
	~window() {}

	static bool init();
	static void destroy();

	static bool should_close();

	static glm::ivec2 get_window_size();
	static GLFWwindow* get_window_handle() { return get().m_window_handle; }
	static void register_resize_callback(resize_callback_t callback);

private:
	static void on_framebuffer_resize(GLFWwindow*, int width, int height);

private:
	GLFWwindow* m_window_handle;
	resize_callback_t m_resize_callback;
};
}  // namespace snova
