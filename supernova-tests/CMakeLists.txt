set(target_name "supernova-tests")

set(src_utils
	"src/utils/singleton.cpp"
	"src/utils/logger.cpp")

set(src_engine
	"src/engine/supernova.cpp")

set(src_files
	${src_engine}
	${src_utils})

add_executable(${target_name} ${src_files})

source_group("src\\engine" FILES ${src_engine})
source_group("src\\utils" FILES ${src_utils})

target_link_libraries(${target_name} "supernova-lib")
target_link_libraries(${target_name} ${GTEST_MAIN_LIBRARY})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")

gtest_discover_tests(${target_name})
cotire(${target_name})
