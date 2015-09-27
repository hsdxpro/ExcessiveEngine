#-------------------------------------------------------------------------------
# SOUND ENGINE
#
# The sound engine is responsible for sound playback, mixing, effects. May have
# multiple implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )

add_subdirectory( SFML )