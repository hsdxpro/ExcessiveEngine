message( "-GraphicsApiGL" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Interfaces/*)

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsApiGL SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( GraphicsApiGL STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries( GraphicsApiGL glew32 opengl32 )
target_link_libraries( GraphicsApiGL Common )

add_executable( GLGapiTest test.cpp )
add_dependencies( GLGapiTest GraphicsApiGL )
target_link_libraries( GLGapiTest Common)

# Link SFML to GraphicsApiGL
# This is only a temporary hack, we should remove this dependency...
#if(DEBUG_BUILD)
#	target_link_libraries( GraphicsApiGL sfml-main-d sfml-system-d sfml-window-d sfml-graphics-d sfml-audio-d sfml-network-d)
#else()
#	target_link_libraries( GraphicsApiGL sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
#endif()