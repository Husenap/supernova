#pragma once

#include "../../precompiled.h"

namespace snova {
class vk_image_view {
public:
	bool init(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels = 1);

	void destroy();

	const VkImageView& get_image_view() const { return m_image_view; }

private:
	VkImageView m_image_view;
};
}  // namespace snova
