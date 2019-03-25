#pragma once

#include "vk_buffer.h"
#include <vulkan/vulkan.h>
#include "../../precompiled.h"

namespace snova {
class vk_image {
public:
	bool init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
	bool init_from_file(const char* image_file);

	void destroy();

	void transition_image_layout(VkImageLayout old_layout, VkImageLayout new_layout);

	const VkImage& get_image() const { return m_image; }
	const VkFormat& get_format() const { return m_format; }

private:
	void copy_buffer_to_image(const vk_buffer& staging_buffer);

private:
	VkImage m_image;
	VkDeviceMemory m_image_memory;
	glm::ivec2 m_dimensions;
	VkFormat m_format;
};
}  // namespace snova
