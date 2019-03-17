#pragma once

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

	void draw_frame();
};
}  // namespace snova
