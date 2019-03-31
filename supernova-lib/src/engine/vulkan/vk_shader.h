#pragma once

#include "../../precompiled.h"

namespace snova{
class vk_shader{
public:
	bool load_shader_from_buffer(const std::vector<char>& buffer);
	bool load_shader_from_file(const char* filepath);

	void destroy();

	const VkShaderModule& get_shader_module() { return m_shader_module; }
private:
	VkShaderModule m_shader_module;
};
}
