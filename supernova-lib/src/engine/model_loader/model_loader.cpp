#include "model_loader.h"

#include <logger/logger.h>

#include <fstream>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

static Assimp::Importer importer;

namespace snova {
std::unique_ptr<model_data_t> model_loader::load_model(const char* model_path) {
	START_TIMER(_model_loader_);

	const aiScene* scene = load_scene(model_path);

	if (!scene) {
		ERROR_LOG("Failed to load aiScene for: %s", model_path);
		return nullptr;
	}

	if (!scene->HasMeshes()) {
		WARNING_LOG("Missing meshes for: %s", model_path);
		return nullptr;
	}

	auto model_data = std::make_unique<model_data_t>();

	for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];

		if (!mesh->HasFaces()) {
			continue;
		}

		model_data->m_vertices.resize(mesh->mNumVertices);
		for (unsigned v_index = 0; v_index < mesh->mNumVertices; ++v_index) {
			vertex_data& vertex = model_data->m_vertices[v_index];
			vertex.pos = {mesh->mVertices[v_index].x, mesh->mVertices[v_index].y, mesh->mVertices[v_index].z};
			vertex.tex_coord = {mesh->mTextureCoords[0][v_index].x, mesh->mTextureCoords[0][v_index].y};
			//memcpy(&model_data->m_vertices[v_index].pos, mesh->mVertices + v_index, sizeof(float) * 3);
			//memcpy(&model_data->m_vertices[v_index].tex_coord, mesh->mTextureCoords + v_index, sizeof(float) * 2);
		}

		model_data->m_indices.reserve(mesh->mNumFaces * 3);
		for (unsigned f_index = 0; f_index < mesh->mNumFaces; ++f_index) {
			aiFace& face = mesh->mFaces[f_index];
			model_data->m_indices.insert(end(model_data->m_indices), face.mIndices, face.mIndices + 3);
		}
	}

	create_vertex_buffer(model_data);
	create_index_buffer(model_data);

	END_TIMER(_model_loader_);
	return model_data;
}

void model_loader::create_vertex_buffer(std::unique_ptr<model_data_t>& model_data) {
	VkDeviceSize buffer_size = sizeof(model_data->m_vertices[0]) * model_data->m_vertices.size();

	vk_buffer staging_buffer;
	staging_buffer.init(buffer_size,
						VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.set_data(model_data->m_vertices.data());

	model_data->m_vertex_buffer.init(buffer_size,
									 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
									 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	model_data->m_vertex_buffer.copy_data_from(staging_buffer);

	staging_buffer.destroy();
}

void model_loader::create_index_buffer(std::unique_ptr<model_data_t>& model_data) {
	VkDeviceSize buffer_size = sizeof(model_data->m_indices[0]) * model_data->m_indices.size();

	vk_buffer staging_buffer;
	staging_buffer.init(buffer_size,
						VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging_buffer.set_data(model_data->m_indices.data());

	model_data->m_index_buffer.init(buffer_size,
									 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
									 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	model_data->m_index_buffer.copy_data_from(staging_buffer);

	staging_buffer.destroy();
}

const aiScene* model_loader::load_scene(const char* model_path) {
	std::ifstream in_file(model_path);
	if (in_file.fail()) {
		ERROR_LOG("Can't find model: %s", model_path);
		return nullptr;
	}
	in_file.close();

	return importer.ReadFile(
		model_path, aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
}

}  // namespace snova
