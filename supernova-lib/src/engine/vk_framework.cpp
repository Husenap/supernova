#include "vk_framework.h"

namespace snova {
vk_framework::vk_framework() {
}

vk_framework::~vk_framework() {
}

bool vk_framework::init() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Herro Triangor";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "SuperNova";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	return true;
}
}  // namespace snova
