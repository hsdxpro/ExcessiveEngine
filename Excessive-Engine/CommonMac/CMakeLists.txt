message( "-CommonMac" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( CommonMac STATIC ${sources} ${headers} )

if(BUILD_EXCESSIVE_DEBUG)
	target_link_libraries( CommonMac sfml-main-d sfml-system-d sfml-window-d sfml-graphics-d sfml-audio-d sfml-network-d)
elseif(NOT BUILD_EXCESSIVE_DEBUG)
	target_link_libraries( CommonMac sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
endif()