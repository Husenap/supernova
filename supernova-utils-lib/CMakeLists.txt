set(target_name "supernova-utils-lib")

set(src_include
	)

set(src_singleton
	"singleton/singleton.h")

set(src_logger
	"logger/logger.h"
	"logger/logger.cpp")

set(src_model_loader
	"model_loader/model_loader.h"
	"model_loader/model_loader.cpp")

set(src_files
	${src_singleton}
	${src_logger}
	${src_model_loader})

add_library(${target_name} STATIC ${src_files})

source_group("src\\singleton" FILES ${src_singleton})
source_group("src\\logger" FILES ${src_logger})
source_group("src\\model_loader" FILES ${src_model_loader})

target_include_directories(${target_name} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

target_link_libraries(${target_name} PUBLIC "compiler-warnings"
											${gfx_libraries})

target_compile_features(${target_name} INTERFACE cxx_std_17)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
