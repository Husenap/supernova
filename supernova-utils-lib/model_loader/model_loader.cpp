#include "model_loader.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <logger/logger.h>

#include <fstream>

namespace snova {
std::unique_ptr<model_loader::model_data_t> model_loader::load_model(const char* model_path) {
	std::ifstream in_file(model_path);
	if (in_file.fail()) {
		ERROR_LOG("Can't find model: %s", model_path);
		return nullptr;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path, aiProcessPreset_TargetRealtime_Fast);
	if (!scene) {
		ERROR_LOG("Failed to load aiScene for: %s", model_path);
		return nullptr;
	}

	auto model_data = std::make_unique<model_loader::model_data_t>();

	if (!scene->HasMeshes()) {
		WARNING_LOG("Missing meshess for: %s", model_path);
		return nullptr;
	}

	return model_data;
}
}  // namespace snova
