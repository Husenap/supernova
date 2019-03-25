#pragma once

#include <vulkan/vulkan.h>

namespace snova {
class vk_sampler {
public:
	bool init(VkFilter filter, VkSamplerAddressMode address_mode);

	void destroy();

	const VkSampler& get_sampler() const { return m_sampler; }

private:
	VkSampler m_sampler;
};
}  // namespace snova
