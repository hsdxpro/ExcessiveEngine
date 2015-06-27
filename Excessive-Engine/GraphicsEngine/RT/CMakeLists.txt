#-------------------------------------------------------------------------------
# GRAPICS ENGINE RASTER
#
# Ray tracing based graphics engine implementation.
#-------------------------------------------------------------------------------


message( "-GraphicsEngineRT" )

#---------------------------------------
# Files

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

#---------------------------------------
# Filters

# Global filter
SOURCE_GROUP(\\ FILES $(sources) $(headers))

#interfaces
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Main
SOURCE_GROUP(Main FILES src/GraphicsEngine.h src/GraphicsEngine.cpp)

#---------------------------------------
# Project

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsEngineRT SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( GraphicsEngineRT STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries(GraphicsEngineRT PlatformLibrary SupportLibrary)
