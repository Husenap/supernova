#pragma once

#include "model_data.h"
#include "../vulkan/vk_buffer.h"

namespace snova {
class model {
public:
	bool load(const char* filepath);
	bool load(const std::optional<model_data>& model);

	void destroy();

	uint32_t get_num_indices() const { return static_cast<uint32_t>(m_model_data.m_indices.size()); }
	const VkBuffer& get_index_buffer() const { return m_index_buffer.get_buffer(); }
	const VkBuffer& get_vertex_buffer() const { return m_vertex_buffer.get_buffer(); }

private:
	void create_vertex_buffer();
	void create_index_buffer();

private:
	model_data m_model_data;
	vk_buffer m_vertex_buffer;
	vk_buffer m_index_buffer;
};
}  // namespace snova
