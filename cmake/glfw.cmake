find_package(OpenGL REQUIRED)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("third_party/glfw")

set(gfx_libraries
	"glfw"
	"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")

set(gfx_includes
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb"
	"$ENV{VULKAN_SDK}/Include")
