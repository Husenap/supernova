#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <cmath>
#include <functional>
#include <array>
#include <optional>
#include <vector>

#include <logger/logger.h>
#include <singleton/singleton.h>

//=======================================
//=== SILENT ALL THIRD PARTY WARNINGS ===
//=======================================
//#pragma warning(push, 0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunused"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wduplicated-cond"
#pragma GCC diagnostic ignored "-Wduplicated-branches"
#pragma GCC diagnostic ignored "-Wlogical-op"
#pragma GCC diagnostic ignored "-Wnull-dereference"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wtype-limits"


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

#pragma GCC diagnostic pop
//#pragma warning(pop)
//=======================================
//=== SILENT ALL THIRD PARTY WARNINGS ===
//=======================================



namespace {
const int MAX_FRAMES_IN_FLIGHT = 2;
}  // namespace
