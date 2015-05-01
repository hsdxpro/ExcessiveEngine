message( "-SoundEngineSFML" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Interfaces/*)

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( SoundEngineSFML SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( SoundEngineSFML "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
else()
	add_library( SoundEngineSFML STATIC ${sources} ${headers} ${interfaces} )
endif()
