set(target_name "supernova-lib")

set(src_precompiled
	"src/precompiled.h")

set(src_include
	"include/supernova/supernova-lib.h")

set(src_engine
	"src/engine/supernova.cpp"
	"src/engine/supernova.h"
	"src/engine/window.cpp"
	"src/engine/window.h")

set(src_files
	${src_precompiled}
	${src_include}
	${src_engine})

add_library(${target_name} STATIC ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\include" FILES ${src_include})
source_group("src\\engine" FILES ${src_engine})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries(${target_name} PUBLIC "compiler-warnings"
											${gfx_libraries})

target_include_directories(${target_name} PUBLIC ${gfx_includes})

target_compile_features(${target_name} INTERFACE cxx_std_17)
