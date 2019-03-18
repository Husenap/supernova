#pragma once

#include "vk_buffer.h"

namespace snova {
class vk_image {
public:
	bool init(const char* image_file);

	void destroy();

	const VkImage& get_image() const { return m_image; }

private:
	void transition_image_layout(VkImageLayout old_layout, VkImageLayout new_layout);

	void copy_buffer_to_image(const vk_buffer& staging_buffer);

private:
	VkImage m_image;
	VkDeviceMemory m_image_memory;
	glm::ivec2 m_dimensions;
};
}  // namespace snova
