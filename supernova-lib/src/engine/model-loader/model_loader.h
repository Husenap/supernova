#pragma once

#include "../model/model_data.h"

namespace snova::model_loader {
std::optional<model_data> load_model(const char* filepath);
}  // namespace snova::model_loader
