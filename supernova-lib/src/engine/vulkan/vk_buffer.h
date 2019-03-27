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
	VkDevice& get_device();

	VkBuffer m_buffer;
	VkDeviceMemory m_buffer_memory;
	VkDeviceSize m_size;
};

template <typename T>
void snova::vk_buffer::set_data(T* in_data) {
	void* mapped_data;
	vkMapMemory(get_device(), m_buffer_memory, 0, m_size, 0, &mapped_data);
	std::copy((char*)in_data, (char*)(in_data)+m_size, (char*)mapped_data);
	vkUnmapMemory(get_device(), m_buffer_memory);
}

}  // namespace snova
