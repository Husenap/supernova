#pragma once

#include <vulkan/vulkan.h>

#include "../../precompiled.h"

namespace snova {
class vk_buffer {
public:
	bool init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	template<typename T>
	void set_data(T* in_data);

	void copy_data_from(const vk_buffer& source);

	void destroy();

	const VkBuffer& get_buffer() const { return m_buffer; }

private:
	uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_buffer_memory;
	VkDeviceSize m_size;
};

template <typename T>
void snova::vk_buffer::set_data(T* in_data) {
	void* mapped_data;
	vkMapMemory(vk_framework::get_device(), m_buffer_memory, 0, m_size, 0, &mapped_data);
	memcpy(mapped_data, (void*)in_data, (size_t)m_size);
	vkUnmapMemory(vk_framework::get_device(), m_buffer_memory);
}

}  // namespace snova