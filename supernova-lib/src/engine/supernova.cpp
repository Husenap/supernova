#include "supernova.h"

#include "../precompiled.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "window.h"

namespace snova {
void supernova::run() {
	if (init()) {
		main_loop();
		destroy();
	}
}

bool supernova::init() {
	if (!window::init()) {
		return false;
	}

	if (!m_vk_framework.init()) {
		return false;
	}

	return true;
}
void supernova::main_loop() {
	while (!window::should_close()) {
		glfwPollEvents();
		draw_frame();
	}
}
void supernova::destroy() {
	m_vk_framework.destroy();
	window::destroy();
}

void supernova::draw_frame() { m_vk_framework.draw_frame(); }

}  // namespace snova
