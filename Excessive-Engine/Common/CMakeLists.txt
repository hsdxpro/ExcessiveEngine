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

if(NOT BUILD_EXCESSIVE_WITH_DLL)
	target_link_libraries( Common GraphicsEngineRaster )
	target_link_libraries( Common GraphicsEngineRT )
	target_link_libraries( Common GraphicsApiGL )
	target_link_libraries( Common PhysicsEngineBullet )
	target_link_libraries( Common SoundEngineSFML )
	target_link_libraries( Common NetworkEngineBoost )
endif()

target_link_libraries( Common assimp)