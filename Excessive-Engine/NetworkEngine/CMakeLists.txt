message( "-NetworkEngine" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Module_Interfaces/NetworkEngine/*)

include_directories("../Module_Interfaces/NetworkEngine/")

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( NetworkEngine SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( NetworkEngine "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( NetworkEngine STATIC ${sources} ${headers} ${interfaces} )
endif()
