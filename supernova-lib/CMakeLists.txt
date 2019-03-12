set(target_name "supernova-lib")

set(src_precompiled
	"src/precompiled.h")

set(src_include
	"include/supernova/supernova-lib.h")

set(src_engine
	"src/supernova.h"
	"src/dummy.cpp")

set(src_files
	${src_precompiled}
	${src_include}
	${src_engine})

add_library(${target_name} STATIC ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\include" FILES ${src_include})
source_group("src\\engine" FILES ${src_engine})

target_link_libraries(${target_name} PUBLIC "compiler-warnings")

target_compile_features(${target_name} INTERFACE cxx_std_17)

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
