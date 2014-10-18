message( "-Common" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( Common STATIC ${sources} ${headers} )