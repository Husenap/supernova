#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "vulkan/vk_buffer.h"

#include <array>
#include <optional>
#include <vector>

#include "singleton.h"

namespace snova {
struct queue_family_indices {
	std::optional<uint32_t> m_graphics_family;
	std::optional<uint32_t> m_present_family;

	bool is_complete() { return m_graphics_family.has_value() && m_present_family.has_value(); }
};

struct swapchain_support_details {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> surface_formats;
	std::vector<VkPresentModeKHR> present_modes;
};

struct vertex_data {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription get_binding_description() {
		VkVertexInputBindingDescription binding_description;
		binding_description.binding = 0;
		binding_description.stride = sizeof(vertex_data);
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return binding_description;
	}

	static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions = {};

		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[0].offset = offsetof(vertex_data, pos);

		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[1].offset = offsetof(vertex_data, color);

		return attribute_descriptions;
	}
};

const std::vector<vertex_data> vertices = {{{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
										   {{+0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
										   {{+0.5f, +0.5f}, {0.0f, 1.0f, 1.0f}},
										   {{-0.5f, +0.5f}, {1.0f, 0.0f, 1.0f}}};

const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

class vk_framework : public singleton<vk_framework> {
public:
	vk_framework();
	~vk_framework();

	bool init();

	void destroy();

	void draw_frame();

	static VkDevice& get_device() { return get().m_device; }
	static VkPhysicalDevice& get_physical_device() { return get().m_physical_device; }
	static VkCommandPool& get_command_pool() { return get().m_command_pool; }
	static VkQueue get_graphics_queue() { return get().m_graphics_queue; }

private:
	bool create_vk_instance();

	bool setup_debug_messenger();

	bool create_surface();

	bool pick_physical_device();
	int rate_device_suitability(VkPhysicalDevice device);
	queue_family_indices find_queue_families(VkPhysicalDevice device);
	bool check_device_extension_support(VkPhysicalDevice device);

	bool create_logical_device();

	bool create_swapchain();
	swapchain_support_details query_swapchain_support(VkPhysicalDevice device);
	VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
	VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes);
	VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

	bool create_image_views();

	bool create_render_pass();

	bool create_graphics_pipeline();
	VkShaderModule create_shader_module(const std::vector<char>& code);

	bool create_frame_buffers();

	bool create_command_pool();

	bool create_vertex_buffer();
	bool create_index_buffer();

	bool create_command_buffers();

	bool create_sync_objects();

	bool recreate_swapchain();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	vlayer_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
					[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type,
					[[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
					[[maybe_unused]] void* p_user_data);

	void destroy_swapchain();

	bool check_validation_layer_support();
	std::vector<const char*> get_requried_extensions();

private:
	VkInstance m_vk_instance;
	VkDebugUtilsMessengerEXT m_debug_messenger;
	VkPhysicalDevice m_physical_device;
	VkDevice m_device;
	VkQueue m_graphics_queue;
	VkQueue m_present_queue;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapchain;
	VkFormat m_swapchain_image_format;
	VkExtent2D m_swapchain_extent;
	std::vector<VkImage> m_swapchain_images;
	std::vector<VkImageView> m_swapchain_image_views;
	std::vector<VkFramebuffer> m_swapchain_framebuffers;
	VkPipelineLayout m_pipeline_layout;
	VkRenderPass m_render_pass;
	VkPipeline m_graphics_pipeline;
	VkCommandPool m_command_pool;
	std::vector<VkCommandBuffer> m_command_buffers;

	vk_buffer m_vertex_buffer;
	vk_buffer m_index_buffer;

	std::vector<VkSemaphore> m_image_available_semaphores;
	std::vector<VkSemaphore> m_render_finished_semaphores;
	std::vector<VkFence> m_in_flight_fences;
	int m_current_frame = 0;

	bool m_framebuffer_resized = false;
};
}  // namespace snova