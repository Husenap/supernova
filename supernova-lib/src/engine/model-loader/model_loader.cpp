#include "model_loader.h"

static Assimp::Importer importer;

namespace snova::model_loader {
std::optional<snova::model_data> load_model(const char* filepath) {
	START_TIMER(_model_loader_);

	std::ifstream in_file(filepath);
	if (in_file.fail()) {
		ERROR_LOG("Can't find model: %s", filepath);
		return std::optional<model_data>{};
	}
	in_file.close();

	const aiScene* scene = importer.ReadFile(
		filepath, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	if (!scene) {
		ERROR_LOG("Failed to load aiScene for: %s", filepath);
		return std::optional<model_data>{};
	}

	if (!scene->HasMeshes()) {
		WARNING_LOG("Missing meshes for: %s", filepath);
		return std::optional<model_data>{};
	}

	model_data data;

	for (unsigned i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];

		if (!mesh->HasFaces()) {
			continue;
		}

		data.m_vertices.resize(mesh->mNumVertices);
		for (unsigned v_index = 0; v_index < mesh->mNumVertices; ++v_index) {
			vertex_data& vertex = data.m_vertices[v_index];
			vertex.pos = {mesh->mVertices[v_index].x, mesh->mVertices[v_index].y, mesh->mVertices[v_index].z};
			vertex.tex_coord = {mesh->mTextureCoords[0][v_index].x, mesh->mTextureCoords[0][v_index].y};

			// memcpy(&model_data->m_vertices[v_index].pos, mesh->mVertices + v_index, sizeof(float) * 3);
			// memcpy(&model_data->m_vertices[v_index].tex_coord, mesh->mTextureCoords + v_index,
			// sizeof(float) * 2);
		}

		data.m_indices.reserve(mesh->mNumFaces * 3);
		for (unsigned f_index = 0; f_index < mesh->mNumFaces; ++f_index) {
			aiFace& face = mesh->mFaces[f_index];
			data.m_indices.insert(end(data.m_indices), face.mIndices, face.mIndices + 3);
		}
	}

	END_TIMER(_model_loader_);
	return data;
}

}  // namespace snova::model_loader
