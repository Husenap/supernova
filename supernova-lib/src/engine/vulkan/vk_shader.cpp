#include "vk_shader.h"

#include "../vk_framework.h"

namespace snova {
bool vk_shader::load_shader_from_buffer(const std::vector<char>& buffer) {
	VkShaderModuleCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = buffer.size();
	create_info.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

	if (vkCreateShaderModule(vk_framework::get_device(), &create_info, nullptr, &m_shader_module) != VK_SUCCESS) {
		ERROR_LOG("Failed to create shader module");
		return false;
	}

	return true;
}
bool vk_shader::load_shader_from_file(const char* filepath) {
	std::ifstream file(filepath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		ERROR_LOG("Failed to open file: %s", filepath);
		return false;
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);

	file.close();

	if (!load_shader_from_buffer(buffer)) {
		ERROR_LOG("Failed to create shader module: %s", filepath);
		return false;
	}

	INFO_LOG("Successfully loaded shader: %s", filepath);
	return true;
}
void vk_shader::destroy() { vkDestroyShaderModule(vk_framework::get_device(), m_shader_module, nullptr); }
}  // namespace snova
