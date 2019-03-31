set(target_name "supernova-tests")

set(src_utils
	"src/utils/singleton.cpp"
	"src/utils/logger.cpp")

set(src_engine
	"src/engine/model_loader.cpp"
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

add_custom_command(TARGET ${target_name}
	POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/res/
	$<TARGET_FILE_DIR:${target_name}>/res/)

set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${target_name}>/")

gtest_discover_tests(${target_name})
cotire(${target_name})
