if (WIN32)
	set(VULKAN_LIBRARY "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
	set(VULKAN_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")
endif()
if (UNIX)
	set(VULKAN_LIBRARY "$ENV{VULKAN_SDK}/lib/libvulkan.so")
	set(VULKAN_INCLUDE_DIR "$ENV{VULKAN_SDK}/include")
endif()


if(NOT VULKAN_LIBRARY OR NOT VULKAN_INCLUDE_DIR)
	message(FATAL_ERROR "Vulkan SDK not located!")
endif()

message("-- Vulkan paths:")
message("-- ${VULKAN_LIBRARY}")
message("-- ${VULKAN_INCLUDE_DIR}")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("third_party/glfw")

set(gfx_libraries
	"glfw"
	"${VULKAN_LIBRARY}")

set(gfx_includes
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb"
	"${VULKAN_INCLUDE_DIR}")

set_target_properties("glfw" PROPERTIES FOLDER "third_party//GLFW")
