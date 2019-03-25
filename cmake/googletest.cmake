message("-- External Project: googletest")
enable_testing()
set(GTEST_PROJECT_FILE "cmake/googletest.cmake.in")

set(GTEST_DIR ${PROJECT_BINARY_DIR}/third_party/googletest/download)
configure_file(${GTEST_PROJECT_FILE} ${GTEST_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${GTEST_DIR}
	OUTPUT_FILE ${GTEST_DIR}/download-log.txt)
if(result)
    message(FATAL_ERROR "googletest download failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${GTEST_DIR}
	OUTPUT_FILE ${GTEST_DIR}/build-log.txt)
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_BINARY_DIR}/third_party/googletest/src)

set(GTEST_FOLDER "third_party//googletest")

set_target_properties(gtest PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gtest_main PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gmock PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gmock_main PROPERTIES FOLDER ${GTEST_FOLDER})

set(GTEST_LIBRARY "gtest")
set(GTEST_MAIN_LIBRARY "gtest_main")
set(GTEST_INCLUDE_DIR "${PROJECT_BINARY_DIR}/third_party/googletest/src/googletest/include")

include(GoogleTest)

include_directories("${gtest_SOURCE_DIR}/include")
