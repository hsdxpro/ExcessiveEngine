message( "-GraphicsApiGL" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

include_directories( "../GraphicsApi_Interface/interface" )

add_library( GraphicsApiGL SHARED ${sources} ${headers} )

add_executable( GLGapiTest src/test.cpp )

target_link_libraries( GLGapiTest GraphicsApiGL sfml-system sfml-window sfml-graphics glew32 )