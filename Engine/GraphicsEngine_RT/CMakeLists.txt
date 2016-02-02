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
FILE( GLOB_RECURSE interfaces "../GraphicsEngine/*.h*")

#---------------------------------------
# Filters

# Global filter
SOURCE_GROUP(\\ FILES $(sources) $(headers))

#interfaces
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Main
SOURCE_GROUP(Main FILES src/GraphicsEngine.h src/GraphicsEngine.cpp)