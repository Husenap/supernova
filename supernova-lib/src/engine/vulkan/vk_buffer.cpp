#include "vk_buffer.h"

#include "../vk_framework.h"

#include "vk_mem_utils.h"

namespace snova {

bool vk_buffer::init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	m_size = size;

	if (vkCreateBuffer(vk_framework::get_device(), &buffer_info, nullptr, &m_buffer) != VK_SUCCESS) {
		FATAL_LOG("Failed to create buffer!");
		return false;
	}

	VkMemoryRequirements mem_requirements;
	vkGetBufferMemoryRequirements(vk_framework::get_device(), m_buffer, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);

	if (vkAllocateMemory(vk_framework::get_device(), &alloc_info, nullptr, &m_buffer_memory) != VK_SUCCESS) {
		FATAL_LOG("Failed to allocate buffer memory!");
		return false;
	}

	vkBindBufferMemory(vk_framework::get_device(), m_buffer, m_buffer_memory, 0);

	return true;
}

void vk_buffer::copy_data_from(const vk_buffer& source) {
	if (source.m_size > m_size) {
		FATAL_LOG("Source size is larger than destination size!");
		return;
	}

	auto command_buffer = begin_single_time_commands();

	VkBufferCopy copy_region = {};
	copy_region.size = source.m_size;
	vkCmdCopyBuffer(command_buffer, source.m_buffer, m_buffer, 1, &copy_region);

	end_single_time_commands(command_buffer);
}

void vk_buffer::destroy() {
	vkDestroyBuffer(vk_framework::get_device(), m_buffer, nullptr);
	vkFreeMemory(vk_framework::get_device(), m_buffer_memory, nullptr);
}

}  // namespace snova