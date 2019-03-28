#pragma once

#include "../../precompiled.h"

namespace snova {
class vk_sampler {
public:
	bool init(VkFilter filter, VkSamplerAddressMode address_mode, uint32_t mip_levels = 1);

	void destroy();

	const VkSampler& get_sampler() const { return m_sampler; }

private:
	VkSampler m_sampler;
};
}  // namespace snova
