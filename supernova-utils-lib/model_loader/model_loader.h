#pragma once

#include <memory>

namespace snova{
class model_loader{
public:
	struct model_data_t {};

	std::unique_ptr<model_data_t> load_model(const char* model_path);

	
private:
};
}
