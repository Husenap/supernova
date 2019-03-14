#include "supernova.h"

#include "../precompiled.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace snova {
void supernova::run() {
	if (init()) {
		main_loop();
		destroy();
	}
}

bool supernova::init() {
	if (!m_window.init()) {
		return false;
	}

	if (!m_vk_framework.init(m_window)) {
		return false;
	}

	return true;
}
void supernova::main_loop() {
	while (!m_window.should_close()) {
		glfwPollEvents();
		draw_frame();
	}
}
void supernova::destroy() {
	m_vk_framework.destroy();
	m_window.destroy();
}

void supernova::draw_frame() { m_vk_framework.draw_frame(); }

}  // namespace snova
