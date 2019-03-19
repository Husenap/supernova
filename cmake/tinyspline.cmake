set(TINYSPLINE_FLOAT_PRECISION ON CACHE BOOL "" FORCE)

add_subdirectory(third_party/tinyspline/src)
add_definitions("${TINYSPLINE_DEFINITIONS}")

set(tinyspline_libraries "tinyspline_shared" "tinyspline_static" "tinysplinecpp_shared" "tinysplinecpp_static")

foreach(library ${tinyspline_libraries})
    set_target_properties(${library} PROPERTIES FOLDER "third_party/tinyspline")
endforeach()
