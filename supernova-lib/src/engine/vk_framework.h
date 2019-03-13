#pragma once

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

namespace snova {
struct queue_family_indices {
	std::optional<uint32_t> m_graphics_family;

	bool is_complete() {
		return m_graphics_family.has_value();
	}
};

class vk_framework {
public:
	vk_framework();
	~vk_framework();

	bool init();

	void destroy();

private:
	bool create_vk_instance();

	bool setup_debug_messenger();

	bool pick_physical_device();
	int rate_device_suitability(VkPhysicalDevice device);
	queue_family_indices find_queue_families(VkPhysicalDevice device);


	static VKAPI_ATTR VkBool32 VKAPI_CALL
	vlayer_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
				   VkDebugUtilsMessageTypeFlagsEXT message_type,
				   const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
				   void* p_user_data);

private:
	bool check_validation_layer_support();
	std::vector<const char*> get_requried_extensions();

	VkInstance m_vk_instance;
	VkDebugUtilsMessengerEXT m_debug_messenger;
	VkPhysicalDevice m_physical_device;
};
}  // namespace snova