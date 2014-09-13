cmake_minimum_required (VERSION 2.6)

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( GraphicsApiGL SHARED ${sources} ${headers} )