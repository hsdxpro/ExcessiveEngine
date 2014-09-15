FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( GraphicsApiGL SHARED ${sources} ${headers} )