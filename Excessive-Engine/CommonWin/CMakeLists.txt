message( "-CommonWin" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( CommonWin STATIC ${sources} ${headers} )

target_link_libraries( CommonWin 
debug sfml-main-d optimized sfml-main
debug sfml-system-d optimized sfml-system
debug sfml-window-d optimized sfml-window
debug sfml-graphics-d optimized sfml-graphics
debug sfml-audio-d optimized sfml-audio
debug sfml-network-d optimized sfml-network )