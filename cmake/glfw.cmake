find_package(OpenGL REQUIRED)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("third_party/glfw")

if (WIN32)
	set(VULKAN_LIB_PATH "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
	set(VULKAN_INC_PATH "$ENV{VULKAN_SDK}/Include")
endif()
if (UNIX)
	set(VULKAN_LIB_PATH "$ENV{VULKAN_SDK}/x86_64/lib/libvulkan.so")
	set(VULKAN_INC_PATH "$ENV{VULKAN_SDK}/x86_64/include")
endif()

set(gfx_libraries
	"glfw"
	"${VULKAN_LIB_PATH}")

set(gfx_includes
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb"
	"${VULKAN_INC_PATH}")

set_target_properties("glfw" PROPERTIES FOLDER "third_party//GLFW")
