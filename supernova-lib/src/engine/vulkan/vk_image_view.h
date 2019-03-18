#pragma once

namespace snova {
class vk_image_view {
public:
	bool init(VkImage image, VkFormat format);

	void destroy();

	const VkImageView& get_image_view() const { return m_image_view; }

private:
	VkImageView m_image_view;
};
}  // namespace snova
