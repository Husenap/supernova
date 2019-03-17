#include "vk_buffer.h"

#include "../vk_framework.h"

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

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = vk_framework::get_command_pool();
	alloc_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;
	if (vkAllocateCommandBuffers(vk_framework::get_device(), &alloc_info, &command_buffer) != VK_SUCCESS) {
		FATAL_LOG("Failed to allocate command buffer");
		return;
	}

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &begin_info);

	VkBufferCopy copy_region = {};
	copy_region.srcOffset = 0;
	copy_region.dstOffset = 0;
	copy_region.size = source.m_size;

	vkCmdCopyBuffer(command_buffer, source.m_buffer, m_buffer, 1, &copy_region);

	vkEndCommandBuffer(command_buffer);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	vkQueueSubmit(vk_framework::get_graphics_queue(), 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vk_framework::get_graphics_queue());

	vkFreeCommandBuffers(vk_framework::get_device(), vk_framework::get_command_pool(), 1, &command_buffer);
}

void vk_buffer::destroy() {
	vkDestroyBuffer(vk_framework::get_device(), m_buffer, nullptr);
	vkFreeMemory(vk_framework::get_device(), m_buffer_memory, nullptr);
}

uint32_t vk_buffer::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(vk_framework::get_physical_device(), &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i) {
		if (type_filter & (1 << i) &&
			(mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	FATAL_LOG("Failed to find suitable memory type");
	return 0;
}

}  // namespace snova