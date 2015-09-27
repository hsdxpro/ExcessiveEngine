#-------------------------------------------------------------------------------
# GRAPICS ENGINE
#
# The graphics engine is responsible for rendering the scene. It has multiple
# implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )

add_subdirectory( Raster )
add_subdirectory( RT )