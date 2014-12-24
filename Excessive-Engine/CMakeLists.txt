message( "-Excessive-Engine" )

add_subdirectory( Common )

if(WIN32)
	add_subdirectory( CommonWin )
elseif(UNIX)
	add_subdirectory( CommonLin )
elseif(APPLE)
	add_subdirectory( CommonMac )
endif()

add_subdirectory( PIL )
add_subdirectory( Core )
add_subdirectory( GraphicsApiGL )
add_subdirectory( GraphicsEngineRaster )
add_subdirectory( GraphicsEngineRT )

add_subdirectory( Test )

add_subdirectory( DLLExample )
add_subdirectory( DLLUser )
