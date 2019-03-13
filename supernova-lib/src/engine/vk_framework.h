#pragma once

#include <vulkan/vulkan.h>

namespace snova {
class vk_framework {
public:
	vk_framework();
	~vk_framework();

	bool init();

private:
	VkInstance m_vk_instance;

};
}  // namespace snova