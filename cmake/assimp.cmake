message("-- External Project: assimp")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE BOOL "" FORCE)

add_subdirectory(third_party/assimp)

if (MSVC)
	set(assimp_libraries
		"assimp"
		"zlib"
		"zlibstatic"
		"IrrXML"
		"UpdateAssimpLibsDebugSymbolsAndDLLs")

	foreach(library ${assimp_libraries})
		set_target_properties(${library} PROPERTIES FOLDER "third_party//assimp")
	endforeach()
endif()
