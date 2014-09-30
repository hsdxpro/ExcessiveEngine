FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../GraphicsApi_Interface/interface/*)

SOURCE_GROUP(Interfaces FILES ${interfaces})

add_library( GraphicsApiGL SHARED ${sources} ${headers} ${interfaces})