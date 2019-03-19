set(TINYSPLINE_FLOAT_PRECISION ON CACHE BOOL "" FORCE)

add_subdirectory(third_party/tinyspline/src)
add_definitions("${TINYSPLINE_DEFINITIONS}")

