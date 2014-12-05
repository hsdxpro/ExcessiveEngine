message( "-Common" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( Common STATIC ${sources} ${headers} )

if(WIN32)
	target_link_libraries( Common CommonWin )
elseif(UNIX)
	target_link_libraries( Common CommonLin )
elseif(APPLE)
	target_link_libraries( Common CommonMac )
endif()