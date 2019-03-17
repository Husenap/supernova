message("-- External Project: cotire")
set(COTIRE_PROJECT_FILE "cmake/cotire.cmake.in")

set(COTIRE_DIR ${PROJECT_BINARY_DIR}/third_party/cotire/download)
configure_file(${COTIRE_PROJECT_FILE} ${COTIRE_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${COTIRE_DIR}
	OUTPUT_FILE ${COTIRE_DIR}/download-log.txt)

if(result)
	message(FATAL_ERROR "cotire download failed: ${result}")
endif()


execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${COTIRE_DIR}
	OUTPUT_FILE ${COTIRE_DIR}/build-log.txt)

if(result)
	message(FATAL_ERROR "build step for cotire failed: ${result}")
endif()

set(COTIRE_TARGETS_FOLDER "third_party//cotire")
include(${PROJECT_BINARY_DIR}/third_party/cotire/src/CMake/cotire.cmake)
