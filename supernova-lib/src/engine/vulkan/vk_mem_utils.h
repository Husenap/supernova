#pragma once

#include <vulkan/vulkan.h>

#include <logger.h>
#include "../vk_framework.h"

namespace snova {
static uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
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

static VkCommandBuffer begin_single_time_commands() {
	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = vk_framework::get_command_pool();
	alloc_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;
	if (vkAllocateCommandBuffers(vk_framework::get_device(), &alloc_info, &command_buffer) != VK_SUCCESS) {
		FATAL_LOG("Failed to allocate command buffer");
	}

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &begin_info);

	return command_buffer;
}
static void end_single_time_commands(VkCommandBuffer command_buffer) {
	vkEndCommandBuffer(command_buffer);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	vkQueueSubmit(vk_framework::get_graphics_queue(), 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(vk_framework::get_graphics_queue());

	vkFreeCommandBuffers(vk_framework::get_device(), vk_framework::get_command_pool(), 1, &command_buffer);
}

}  // namespace snova
