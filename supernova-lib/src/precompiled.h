#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <vector>

#include <logger/logger.h>
#include <singleton/singleton.h>

namespace {
const int MAX_FRAMES_IN_FLIGHT = 2;
}  // namespace


#include <silent-warnings/begin.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <silent-warnings/end.h>
