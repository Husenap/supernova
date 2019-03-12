set(target_name "supernova-main")

set(src_files
	"src/main.cpp")

add_executable(${target_name} ${src_files})

source_group("src\\supernova" FILES ${src_files})

target_link_libraries(${target_name} "supernova-lib")

target_include_directories(${target_name} PRIVATE "../supernova-lib/include")

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
