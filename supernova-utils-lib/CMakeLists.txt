set(target_name "supernova-utils-lib")

set(src_include
	"include/logger.h"
	"include/singleton.h"
	)

set(src_utils
	"src/logger.cpp")

set(src_files
	${src_include}
	${src_utils})

add_library(${target_name} STATIC ${src_files})

source_group("src" FILES ${src_include} ${src_utils})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries(${target_name} PUBLIC "compiler-warnings"
											${gfx_libraries})

target_compile_features(${target_name} INTERFACE cxx_std_17)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
