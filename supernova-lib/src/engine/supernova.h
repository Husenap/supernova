#pragma once

#include "window.h"
#include "vk_framework.h"

namespace snova {
class supernova {
public:
	supernova() {
	}
	~supernova() {
	}

	void run();

private:
	bool init();
	void main_loop();
	void destroy();

	void draw_frame();

	window m_window;
	vk_framework m_vk_framework;
};
}  // namespace snova
