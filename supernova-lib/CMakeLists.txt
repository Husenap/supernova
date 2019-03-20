set(target_name "supernova-lib")

set(src_precompiled
	"src/precompiled.h")

set(src_include
	"include/supernova/supernova-lib.h")

set(src_engine
	"src/engine/vk_framework.cpp"
	"src/engine/vk_framework.h"
	"src/engine/supernova.cpp"
	"src/engine/supernova.h"
	"src/engine/window.cpp"
	"src/engine/window.h")

set(src_vulkan
	"src/engine/vulkan/vk_mem_utils.h"
	"src/engine/vulkan/vk_sampler.cpp"
	"src/engine/vulkan/vk_sampler.h"
	"src/engine/vulkan/vk_image_view.cpp"
	"src/engine/vulkan/vk_image_view.h"
	"src/engine/vulkan/vk_image.cpp"
	"src/engine/vulkan/vk_image.h"
	"src/engine/vulkan/vk_buffer.cpp"
	"src/engine/vulkan/vk_buffer.h")

set(src_files
	${src_precompiled}
	${src_include}
	${src_engine}
	${src_vulkan})

add_library(${target_name} STATIC ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\include" FILES ${src_include})
source_group("src\\engine" FILES ${src_engine})
source_group("src\\engine\\vulkan" FILES ${src_vulkan})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${src_precompiled})

target_link_libraries(${target_name} PUBLIC
	${gfx_libraries}
	"compiler-warnings"
	"supernova-utils-lib")

target_include_directories(${target_name} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${gfx_includes})

target_compile_features(${target_name} INTERFACE cxx_std_17)

cotire(${target_name})
