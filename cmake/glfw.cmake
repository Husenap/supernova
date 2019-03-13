find_package(OpenGL REQUIRED)

add_subdirectory("third_party/glfw")

set(gfx_libraries
	"glfw"
	"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")

set(gfx_includes
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/deps"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
	"$ENV{VULKAN_SDK}/Include")
