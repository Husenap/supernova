#include "supernova.h"

#include "../precompiled.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace snova {
void supernova::run() {
	if (init()) {
		main_loop();
		cleanup();
	}
}

bool supernova::init() {
	if (!m_window.init()) {
		FATAL_LOG("Failed to init window");
		return false;
	}

	return true;
}
void supernova::main_loop() {
	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	while (!m_window.should_close()) {
		glfwPollEvents();
	}

	cleanup();
}
void supernova::cleanup() {
	m_window.destroy();
}

}  // namespace snova
