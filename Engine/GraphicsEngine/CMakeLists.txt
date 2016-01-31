#-------------------------------------------------------------------------------
# GRAPICS ENGINE
#
# The graphics engine is responsible for rendering the scene. It has multiple
# implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )

FILE(GLOB_RECURSE h_Raster Raster/*.c*)
FILE(GLOB_RECURSE s_Raster Raster/*.h*)

FILE(GLOB_RECURSE h_RT RT/*.h*)
FILE(GLOB_RECURSE s_RT RT/*.c*)

source_group(Interfaces FILES ${interfaces})
source_group(Raster FILES ${h_Raster} ${s_Raster})
source_group(RT FILES ${h_RT} ${s_RT})

add_library( GraphicsEngine STATIC ${h_Raster} ${s_Raster} ${h_RT} ${s_RT} ${interfaces} )
target_link_libraries(GraphicsEngine PlatformLibrary SupportLibrary)

add_subdirectory( Raster )
add_subdirectory( RT )
