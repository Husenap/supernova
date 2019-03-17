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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
// Todo: Replace these with settings
const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;
}  // namespace