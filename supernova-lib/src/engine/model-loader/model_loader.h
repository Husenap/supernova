#pragma once

#include "../../precompiled.h"

#include "../vulkan/vk_buffer.h"

struct aiScene;

namespace snova {

struct vertex_data {
	glm::vec3 pos;
	glm::vec2 tex_coord;

	static VkVertexInputBindingDescription get_binding_description() {
		VkVertexInputBindingDescription binding_description;
		binding_description.binding = 0;
		binding_description.stride = sizeof(vertex_data);
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return binding_description;
	}

	static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions = {};

		attribute_descriptions[0].binding = 0;
		attribute_descriptions[0].location = 0;
		attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions[0].offset = offsetof(vertex_data, pos);

		attribute_descriptions[1].binding = 0;
		attribute_descriptions[1].location = 1;
		attribute_descriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attribute_descriptions[1].offset = offsetof(vertex_data, tex_coord);

		return attribute_descriptions;
	}
};

struct model_data_t {
	std::vector<vertex_data> m_vertices;
	std::vector<uint32_t> m_indices;
	vk_buffer m_vertex_buffer;
	vk_buffer m_index_buffer;
};

class model_loader {
public:
	std::unique_ptr<model_data_t> load_model(const char* model_path);


private:
	const aiScene* load_scene(const char* model_path);
	void create_vertex_buffer(std::unique_ptr<model_data_t>& model_data);
	void create_index_buffer(std::unique_ptr<model_data_t>& model_data);
};
}  // namespace snova
