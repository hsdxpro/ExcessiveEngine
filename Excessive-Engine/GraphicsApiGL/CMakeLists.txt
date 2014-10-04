message( "-GraphicsApiGL" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../GraphicsApi_Interface/interface/*)

include_directories( "../GraphicsApi_Interface/interface/" )

add_library( GraphicsApiGL SHARED ${sources} ${headers} ${interfaces} )

target_link_libraries( GraphicsApiGL glew32 opengl32 )

add_executable( GLGapiTest test.cpp )
add_dependencies( GLGapiTest GraphicsApiGL )

target_link_libraries( GLGapiTest 
debug sfml-main-d optimized sfml-main
debug sfml-system-d optimized sfml-system
debug sfml-window-d optimized sfml-window
debug sfml-graphics-d optimized sfml-graphics
debug sfml-audio-d optimized sfml-audio
debug sfml-network-d optimized sfml-network )
