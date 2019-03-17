#include "supernova.h"

#include "../precompiled.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "window.h"
#include "vk_framework.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

	if (!vk_framework::get().init()) {
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
	vk_framework::get().destroy();
	window::destroy();
}

void supernova::draw_frame() { vk_framework::get().draw_frame(); }

}  // namespace snova
