#include "vk_image_view.h"

#include <logger/logger.h>

#include "../vk_framework.h"

namespace snova {
bool vk_image_view::init(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags) {
	VkImageViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;
	view_info.subresourceRange.aspectMask = aspect_flags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = 1;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = 1;

	if (vkCreateImageView(vk_framework::get_device(), &view_info, nullptr, &m_image_view) != VK_SUCCESS) {
		FATAL_LOG("Failed to create image view!");
		return false;
	}

	VERBOSE_LOG("Created image view");
	return true;
}
void vk_image_view::destroy() { vkDestroyImageView(vk_framework::get_device(), m_image_view, nullptr); }
}  // namespace snova