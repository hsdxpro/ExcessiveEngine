message( "-NetworkEngineBoost" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Interfaces/*)

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( NetworkEngineBoost SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( NetworkEngineBoost "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( NetworkEngineBoost STATIC ${sources} ${headers} ${interfaces} )
endif()
