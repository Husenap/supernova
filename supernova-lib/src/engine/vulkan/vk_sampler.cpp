#include "vk_sampler.h"

#include <logger/logger.h>

#include "../vk_framework.h"

namespace snova {
bool vk_sampler::init(VkFilter filter, VkSamplerAddressMode address_mode) {
	VkSamplerCreateInfo sampler_info = {};
	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = filter;
	sampler_info.minFilter = filter;
	sampler_info.addressModeU = address_mode;
	sampler_info.addressModeV = address_mode;
	sampler_info.addressModeW = address_mode;
	sampler_info.anisotropyEnable = VK_FALSE;
	sampler_info.maxAnisotropy = 1;
	sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = VK_FALSE;
	sampler_info.compareEnable = VK_FALSE;
	sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.mipLodBias = 0.0f;
	sampler_info.minLod = 0.0f;
	sampler_info.maxLod = 0.0f;

	if (vkCreateSampler(vk_framework::get_device(), &sampler_info, nullptr, &m_sampler) != VK_SUCCESS) {
		FATAL_LOG("Failed to create sampler!");
		return false;
	}

	VERBOSE_LOG("Created sampler");
	return true;
}
void vk_sampler::destroy() { vkDestroySampler(vk_framework::get_device(), m_sampler, nullptr); }
}  // namespace snova
