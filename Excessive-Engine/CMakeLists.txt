#-------------------------------------------------------------------------------
# EXCESSIVE-ENGINE MODULES
#
# Excessive-Engine's core modules are included here.
#-------------------------------------------------------------------------------

message( "-Excessive-Engine" )

# configure include paths
include_directories( ${CMAKE_CURRENT_LIST_DIR} )

# add modules
add_subdirectory( PlatformLibrary )
add_subdirectory( SupportLibrary )
add_subdirectory( Core )
add_subdirectory( GraphicsApi )
add_subdirectory( GraphicsEngine )
add_subdirectory( PhysicsEngine )
add_subdirectory( SoundEngine )
add_subdirectory( NetworkEngine )

