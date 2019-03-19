set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SPIRV_HEADERS_DIR $ENV{VULKAN_SDK}/spirv-tools)
set(SHADERC_GLSLANG_DIR $ENV{VULKAN_SDK}/glslang)

add_subdirectory("third_party/glfw")
add_subdirectory("third_party/shaderc")

set(gfx_libraries
	"glfw"
	"shaderc"
	"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")

set(gfx_includes
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb"
	"$ENV{VULKAN_SDK}/Include")
