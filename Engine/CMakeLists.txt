#-------------------------------------------------------------------------------
# EXCESSIVE-ENGINE MODULES
#
# Excessive-Engine's core modules are included here.
#-------------------------------------------------------------------------------

message( "-Engine" )

# add modules
add_subdirectory( BaseLibrary )
add_subdirectory( PlatformLibrary )
add_subdirectory( SupportLibrary )

add_subdirectory( Core )

add_subdirectory( GraphicsApi_OpenGL )
#add_subdirectory( GraphicsApi_DX11 )

add_subdirectory( GraphicsEngine_Raster )
add_subdirectory( GraphicsEngine_RT )

add_subdirectory( PhysicsEngine_Bullet )

add_subdirectory( SoundEngine_SFML )

add_subdirectory( NetworkEngine_Raknet )

