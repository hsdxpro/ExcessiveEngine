message( "-CommonLin" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( CommonLin STATIC ${sources} ${headers} )


target_link_libraries( CommonLin
debug sfml-main-d optimized sfml-main
debug sfml-system-d optimized sfml-system-s
debug sfml-window-d optimized sfml-window-s
debug sfml-graphics-d optimized sfml-graphics-s
debug sfml-audio-d optimized sfml-audio-s
debug sfml-network-d optimized sfml-network-s )