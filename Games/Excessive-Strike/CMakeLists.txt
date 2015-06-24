message( "-Excessive-Strike" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_executable( Excessive-Strike ${sources} ${headers} )

target_link_libraries( Excessive-Strike Core)
