#pragma once

#include "DirectXFramework.h"
#include "WindowHandler.h"

namespace snova {
class supernova {
public:
	supernova() {}
	~supernova() {}

	void run();

private:
	bool init();
	void main_loop();
	void destroy();

	CWindowHandler m_window_handler;
	CDirectXFramework m_framework;
};
}  // namespace snova
