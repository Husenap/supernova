#pragma once

#include "../../precompiled.h"

#include "vertex_data.h"

namespace snova {
struct model_data {
	std::vector<vertex_data> m_vertices;
	std::vector<uint32_t> m_indices;
};
}  // namespace snova
