#-------------------------------------------------------------------------------
# PHYSICS ENGINE
#
# Physics engine is responsible for all game physics simulation. May have multiple
# implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )
SOURCE_GROUP( Interfaces FILES ${interfaces})

add_subdirectory( Bullet )