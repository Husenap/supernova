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
	"src/engine/vulkan/vk_graphics_pipeline.cpp"
	"src/engine/vulkan/vk_graphics_pipeline.h"
	"src/engine/vulkan/vk_shader.cpp"
	"src/engine/vulkan/vk_shader.h"
	"src/engine/vulkan/vk_sampler.cpp"
	"src/engine/vulkan/vk_sampler.h"
	"src/engine/vulkan/vk_image_view.cpp"
	"src/engine/vulkan/vk_image_view.h"
	"src/engine/vulkan/vk_image.cpp"
	"src/engine/vulkan/vk_image.h"
	"src/engine/vulkan/vk_buffer.cpp"
	"src/engine/vulkan/vk_buffer.h")

set(src_model
	"src/engine/model/model.cpp"
	"src/engine/model/model.h"
	"src/engine/model/model_data.h"
	"src/engine/model/vertex_data.h")

set(src_model_loader
	"src/engine/model-loader/model_loader.h"
	"src/engine/model-loader/model_loader.cpp")


set(src_files
	"src/precompiled.cpp"
	${src_precompiled}
	${src_include}
	${src_engine}
	${src_vulkan}
	${src_model}
	${src_model_loader})

add_library(${target_name} STATIC ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\include" FILES ${src_include})
source_group("src\\engine" FILES ${src_engine})
source_group("src\\engine\\vulkan" FILES ${src_vulkan})
source_group("src\\engine\\model" FILES ${src_model})
source_group("src\\engine\\model-loader" FILES ${src_model_loader})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${src_precompiled})

target_link_libraries(${target_name} PUBLIC ${gfx_libraries})
target_link_libraries(${target_name} PUBLIC "compiler-warnings")
target_link_libraries(${target_name} PUBLIC "supernova-utils-lib")
target_link_libraries(${target_name} PUBLIC "supernova-coverage")

target_include_directories(${target_name} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${gfx_includes})

target_compile_features(${target_name} INTERFACE cxx_std_17)

cotire(${target_name})
