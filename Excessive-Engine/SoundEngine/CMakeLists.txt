message( "-SoundEngine" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Module_Interfaces/SoundEngine/*)

include_directories("../Module_Interfaces/SoundEngine/")

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( SoundEngine SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( SoundEngine "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( SoundEngine STATIC ${sources} ${headers} ${interfaces} )
endif()
