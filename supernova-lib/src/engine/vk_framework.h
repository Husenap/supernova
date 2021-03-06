#pragma once

#include "vulkan/vk_buffer.h"
#include "vulkan/vk_image.h"
#include "vulkan/vk_image_view.h"
#include "vulkan/vk_sampler.h"
#include "vulkan/vk_graphics_pipeline.h"

#include "model/model.h"

#include "../precompiled.h"

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

struct uniform_buffer_object {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
	glm::mat4 inv_proj;
	glm::mat4 inv_viewproj;
	glm::vec4 time;
	glm::vec4 camera_pos;
};

class vk_framework : public singleton<vk_framework> {
public:
	vk_framework();
	~vk_framework();

	bool init();

	void destroy();

	void draw_frame();

	inline static VkDevice& get_device() { return get().m_device; }
	inline static VkPhysicalDevice& get_physical_device() { return get().m_physical_device; }
	inline static VkCommandPool& get_command_pool() { return get().m_command_pool; }
	inline static VkQueue get_graphics_queue() { return get().m_graphics_queue; }

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

	bool create_descriptor_set_layout();

	bool create_graphics_pipeline();

	bool create_framebuffers();

	bool create_command_pool();

	bool create_depth_resources();
	bool create_texture_image();
	bool create_model();
	bool create_uniform_buffers();
	bool create_descriptor_pool();
	bool create_descriptor_sets();

	bool create_command_buffers();
	bool create_command_buffer(uint32_t current_image);

	bool create_sync_objects();

	bool recreate_swapchain();

	static VKAPI_ATTR VkBool32 VKAPI_CALL
	vlayer_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
					[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT message_type,
					[[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
					[[maybe_unused]] void* p_user_data);

	void destroy_swapchain();

	void update_uniform_buffer(uint32_t current_image);

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
	std::vector<vk_image_view> m_swapchain_image_views;
	std::vector<VkFramebuffer> m_swapchain_framebuffers;
	VkDescriptorSetLayout m_descriptor_set_layout;
	VkRenderPass m_render_pass;
	vk_graphics_pipeline m_graphics_pipeline;
	VkCommandPool m_command_pool;
	std::vector<VkCommandBuffer> m_command_buffers;

	model m_model;
	std::vector<vk_buffer> m_uniform_buffers;

	model m_quad;

	vk_image m_texture_image;
	vk_image_view m_texture_image_view;
	vk_sampler m_texture_sampler;

	vk_image m_depth_image;
	vk_image_view m_depth_image_view;

	VkDescriptorPool m_descriptor_pool;
	std::vector<VkDescriptorSet> m_descriptor_sets;

	std::vector<VkSemaphore> m_image_available_semaphores;
	std::vector<VkSemaphore> m_render_finished_semaphores;
	std::vector<VkFence> m_in_flight_fences;
	int m_current_frame = 0;

	bool m_framebuffer_resized = false;
};
}  // namespace snova
