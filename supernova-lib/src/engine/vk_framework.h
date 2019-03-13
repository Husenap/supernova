#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace snova {
class vk_framework {
public:
	vk_framework();
	~vk_framework();

	bool init();

	void destroy();

private:
	bool create_vk_instance();
	bool setup_debug_messenger();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
				   VkDebugUtilsMessageTypeFlagsEXT message_type,
				   const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
				   void* p_user_data);

private:
	bool check_validation_layer_support();
	std::vector<const char*> get_requried_extensions();

	VkInstance m_vk_instance;
	VkDebugUtilsMessengerEXT m_debug_messenger;
};
}  // namespace snova