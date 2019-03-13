#pragma once

#include <vulkan/vulkan.h>

#include "window.h"

#include <optional>
#include <vector>

namespace snova {
struct queue_family_indices {
	std::optional<uint32_t> m_graphics_family;
	std::optional<uint32_t> m_present_family;

	bool is_complete() {
		return m_graphics_family.has_value() && m_present_family.has_value();
	}
};

class vk_framework {
public:
	vk_framework();
	~vk_framework();

	bool init(const snova::window& window);

	void destroy();

private:
	bool create_vk_instance();

	bool setup_debug_messenger();

	bool create_surface(const snova::window& window);

	bool pick_physical_device();
	int rate_device_suitability(VkPhysicalDevice device);
	queue_family_indices find_queue_families(VkPhysicalDevice device);

	bool create_logical_device();

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
	VkDevice m_device;
	VkQueue m_graphics_queue;
	VkSurfaceKHR m_surface;
};
}  // namespace snova