#-------------------------------------------------------------------------------
# EXCESSIVE-ENGINE MODULES
#
# Excessive-Engine's core modules are included here.
#-------------------------------------------------------------------------------

message( "-Engine" )

# add modules
add_subdirectory( PlatformLibrary )
add_subdirectory( SupportLibrary )
add_subdirectory( Core )
add_subdirectory( Gapi )
add_subdirectory( GraphicsEngine )
add_subdirectory( PhysicsEngine )
add_subdirectory( SoundEngine )
add_subdirectory( NetworkEngine )

