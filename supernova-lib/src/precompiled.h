#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <set>

#include <logger.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>


namespace {
const int MAX_FRAMES_IN_FLIGHT = 2;
}  // namespace