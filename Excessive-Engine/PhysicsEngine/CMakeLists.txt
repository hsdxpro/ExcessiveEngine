#-------------------------------------------------------------------------------
# PHYSICS ENGINE
#
# Physics engine is responsible for all game physics simulation. May have multiple
# implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )

add_subdirectory( Bullet )