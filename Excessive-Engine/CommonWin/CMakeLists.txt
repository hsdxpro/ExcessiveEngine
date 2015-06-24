message( "-CommonWin" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( CommonWin STATIC ${sources} ${headers} )

target_link_libraries( CommonWin glew32 opengl32 )

if(DEBUG_BUILD)
	target_link_libraries( CommonWin sfml-main-d sfml-system-d sfml-window-d sfml-graphics-d sfml-audio-d sfml-network-d)
else()
	target_link_libraries( CommonWin sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
endif()