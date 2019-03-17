#include "vk_image.h"

#include "../vk_framework.h"
#include "vk_mem_utils.h"

namespace snova {
bool vk_image::init(const char* image_file) {
	glm::ivec3 image_data;
	stbi_uc* pixels = stbi_load(image_file, &image_data.x, &image_data.y, &image_data.z, STBI_rgb_alpha);

	if (!pixels) {
		FATAL_LOG("Failed to load texture image!");
	}

	m_dimensions = {image_data.x, image_data.y};
	VkDeviceSize image_size = image_data.x * image_data.y * 4;

	vk_buffer staging_buffer;
	staging_buffer.init(image_size,
						VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.set_data(pixels);

	stbi_image_free(pixels);

	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = static_cast<uint32_t>(image_data.x);
	image_info.extent.height = static_cast<uint32_t>(image_data.y);
	image_info.extent.depth = 1;
	image_info.mipLevels = 1;
	image_info.arrayLayers = 1;
	image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
	image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = VK_SAMPLE_COUNT_1_BIT;

	if (vkCreateImage(vk_framework::get_device(), &image_info, nullptr, &m_image) != VK_SUCCESS) {
		FATAL_LOG("Failed to create image!");
		return false;
	}

	VkMemoryRequirements mem_requirements;
	vkGetImageMemoryRequirements(vk_framework::get_device(), m_image, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex =
		find_memory_type(mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(vk_framework::get_device(), &alloc_info, nullptr, &m_image_memory) != VK_SUCCESS) {
		FATAL_LOG("Failed to allocate image memory!");
		return false;
	}

	vkBindImageMemory(vk_framework::get_device(), m_image, m_image_memory, 0);

	transition_image_layout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copy_buffer_to_image(staging_buffer);
	transition_image_layout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	staging_buffer.destroy();

	INFO_LOG("Successfully loaded image %s", image_file);
	return true;
}

void vk_image::destroy() {
	vkDestroyImage(vk_framework::get_device(), m_image, nullptr);
	vkFreeMemory(vk_framework::get_device(), m_image_memory, nullptr);
}

void vk_image::transition_image_layout(VkImageLayout old_layout, VkImageLayout new_layout) {
	auto command_buffer = begin_single_time_commands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags source_stage = 0;
	VkPipelineStageFlags destination_stage = 0;

	if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			   new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		FATAL_LOG("Unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	end_single_time_commands(command_buffer);
}

void vk_image::copy_buffer_to_image(const vk_buffer& staging_buffer) {
	auto command_buffer = begin_single_time_commands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {static_cast<uint32_t>(m_dimensions.x), static_cast<uint32_t>(m_dimensions.y), 1};

	vkCmdCopyBufferToImage(command_buffer,
						   staging_buffer.get_buffer(),
						   m_image,
						   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						   1,
						   &region);

	end_single_time_commands(command_buffer);
}

}  // namespace snova
