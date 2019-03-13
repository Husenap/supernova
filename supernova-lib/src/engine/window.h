#pragma once

typedef struct GLFWwindow GLFWwindow;

namespace snova {
class window {
public:
	window() {}
	~window() {}

	bool init();

	bool should_close();

	void destroy();

	GLFWwindow* get_window_handle() const { return m_window_handle; }

private:
	GLFWwindow* m_window_handle;
};
}  // namespace snova