#-------------------------------------------------------------------------------
# CORE
#
# Ricsi kérlek ide írj valami rövid információt!
#-------------------------------------------------------------------------------

message( "-Core: @RICSI A CMAKELISTS-BE IRJ FILE HEADERT!" )

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_library( Core STATIC ${sources} ${headers} )

target_link_libraries( Core SupportLibrary PlatformLibrary )

target_link_libraries( Core GraphicsApi_OpenGL
							GraphicsEngine_Raster
							GraphicsEngine_RT
							NetworkEngine_Raknet
							PhysicsEngine_Bullet
							SoundEngine_SFML )