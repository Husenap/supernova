#pragma once

#include "window.h"

namespace snova {
class supernova {
public:
	supernova() {}
	~supernova() {}

	void run();

private:
	bool init();
	void main_loop();
	void cleanup();

	window m_window;
};
}  // namespace snova
