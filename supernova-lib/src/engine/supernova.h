#pragma once

#include "window.h"

namespace snova {
class supernova {
public:
	supernova() {}
	~supernova() {}

	bool init();

	void run();

private:
	void destroy();

	window m_window;
};
}  // namespace sn
