set(external_project "tinyspline")

message("-- External Project: ${external_project}")
set(EXTERNAL_PROJECT_FILE "cmake/${external_project}.cmake.in")

set(EXTERNAL_PROJECT_DIR ${PROJECT_BINARY_DIR}/third_party/${external_project}/download)
configure_file(${EXTERNAL_PROJECT_FILE} ${EXTERNAL_PROJECT_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G  "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE reuslt
    WORKING_DIRECTORY ${EXTERNAL_PROJECT_DIR}
    OUTPUT_FILE ${EXTERNAL_PROJECT_DIR}/download-log.txt)

if(result)
	message(FATAL_ERROR "${external_project} download failed: ${result}")
endif()

set(TINYSPLINE_FLOAT_PRECISION ON)

execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
    WORKING_DIRECTORY ${EXTERNAL_PROJECT_DIR}
    OUTPUT_FILE ${EXTERNAL_PROJECT_DIR}/build-log.txt)

if(result)
	message(FATAL_ERROR "build step for ${external_project} failed: ${result}")
endif()

add_subdirectory(${PROJECT_BINARY_DIR}/third_party/${external_project}/src/src)

set(tinyspline_libraries "tinyspline_shared" "tinyspline_static" "tinysplinecpp_shared" "tinysplinecpp_static")

foreach(library ${tinyspline_libraries})
    set_target_properties(${library} PROPERTIES FOLDER "third_party/tinyspline")
endforeach()
