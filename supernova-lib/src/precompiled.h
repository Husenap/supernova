#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <set>

#include <logger.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#	pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4201)  // nonstandard extension used : nameless struct/union
#elif defined(__GNUC__) || defined(__MINGW32__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wpedantic"
#endif
#include <stb_image.h>
#if defined(__clang__)
#	pragma clang diagnostic pop
#elif defined(_MSC_VER)
#	pragma warning(pop)
#elif defined(__GNUC__) || defined(__MINGW32__)
#	pragma GCC diagnostic pop
#endif

namespace {
const int MAX_FRAMES_IN_FLIGHT = 2;
}  // namespace