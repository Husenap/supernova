#include "model.h"

#include "../model-loader/model_loader.h"

namespace snova {
bool model::load(const char* filepath) {
	std::optional<model_data> model = model_loader::load_model(filepath);
	if (!model.has_value()) {
		ERROR_LOG("Failed to load model: %s", filepath);
	}

	m_model_data = model.value();

	create_vertex_buffer();
	create_index_buffer();

	return true;
}

void model::destroy() {
	m_index_buffer.destroy();
	m_vertex_buffer.destroy();
}

void model::create_vertex_buffer() {
	VkDeviceSize buffer_size = sizeof(m_model_data.m_vertices[0]) * m_model_data.m_vertices.size();

	vk_buffer staging_buffer;
	staging_buffer.init(buffer_size,
						VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.set_data(m_model_data.m_vertices.data());

	m_vertex_buffer.init(buffer_size,
						 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
						 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_vertex_buffer.copy_data_from(staging_buffer);

	staging_buffer.destroy();
}

void model::create_index_buffer() {
	VkDeviceSize buffer_size = sizeof(m_model_data.m_indices[0]) * m_model_data.m_indices.size();

	vk_buffer staging_buffer;
	staging_buffer.init(buffer_size,
						VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.set_data(m_model_data.m_indices.data());

	m_index_buffer.init(buffer_size,
						VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
						VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_index_buffer.copy_data_from(staging_buffer);

	staging_buffer.destroy();
}

}  // namespace snova