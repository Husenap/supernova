set(target_name "supernova-tests")

set(src_precompiled
	"src/precompiled.h")

set(src_tests
	"src/tests.cpp")

set(src_files
	${src_tests})

add_executable(${target_name} ${src_files})

source_group("src\\tests" FILES ${src_tests})

target_link_libraries(${target_name} "supernova-lib")
target_link_libraries(${target_name} ${GTEST_MAIN_LIBRARY})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${src_precompiled})

gtest_discover_tests(${target_name})
cotire(${target_name})
