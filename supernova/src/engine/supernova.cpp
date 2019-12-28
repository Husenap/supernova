#include "supernova.h"

namespace snova {
void supernova::run() {
	if (init()) {
		main_loop();
		destroy();
	}
}

bool supernova::init() {
	m_window_handler.Init();
	if (!m_framework.Init(m_window_handler)) {
		return false;
	}

	return true;
}
void supernova::main_loop() {
	MSG windowsMessage = {0};

	while (!m_window_handler.ShouldClose()) {
		while (PeekMessage(&windowsMessage, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&windowsMessage);
			DispatchMessage(&windowsMessage);
		}

		m_framework.Present();
	}
}
void supernova::destroy() { m_window_handler.CloseWindow(); }

}  // namespace snova
