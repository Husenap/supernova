set(target_name "supernova")

set(src_precompiled
	"src/precompiled.h")

set(src_engine
	"src/engine/supernova.cpp"
	"src/engine/supernova.h")

set(src_dx
	"src/engine/WindowHandler.h"
	"src/engine/WindowHandler.cpp"
	"src/engine/DirectXFramework.h"
	"src/engine/DirectXFramework.cpp")

set(src_utils
	"utils/singleton/singleton.h"
	"utils/logger/logger.h"
	"utils/logger/logger.cpp"
	"utils/math/Bounds.h"
	"utils/math/Plane.h"
	"utils/math/Frustum.cpp"
	"utils/math/Frustum.h"
	"utils/math/Mathf.cpp"
	"utils/math/Mathf.h"
	"utils/math/Matrix.h"
	"utils/math/Matrix33.h"
	"utils/math/Matrix44.h"
	"utils/math/Vector.h"
	"utils/math/Vector2.h"
	"utils/math/Vector3.h"
	"utils/math/Vector4.h")

set(src_files
	"src/main.cpp"
	${src_precompiled}
	${src_engine}
	${src_dx}
	${src_utils})

add_executable(${target_name} ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\engine" FILES ${src_engine})
source_group("src\\dx" FILES ${src_dx})
source_group("src\\utils" FILES ${src_utils})

set_target_properties(${target_name} PROPERTIES FOLDER "supernova")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 17)
set_target_properties(${target_name} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${src_precompiled})
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

target_compile_features(${target_name} INTERFACE cxx_std_17)

target_link_libraries(${target_name}
	d3d11.lib
	d3dcompiler.lib
	dxgi.lib
)

target_compile_options(${target_name} PUBLIC
	"/Os"
)

cotire(${target_name})
