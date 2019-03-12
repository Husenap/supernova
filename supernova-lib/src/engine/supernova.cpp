#include "supernova.h"

#include <iostream>

#include "../precompiled.h"

namespace snova {
bool supernova::init() {
	if (!m_window.init()) {
		std::cerr << "Failed to init window" << std::endl;
		return false;
	}

	return true;
}
void supernova::run() {
	while (!m_window.should_close()) {
		glfwPollEvents();
	}

	destroy();
}
void supernova::destroy() {
	m_window.destroy();
}
}  // namespace snova
