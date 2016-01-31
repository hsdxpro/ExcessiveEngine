#-------------------------------------------------------------------------------
# SOUND ENGINE
#
# The sound engine is responsible for sound playback, mixing, effects. May have
# multiple implementations.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE( GLOB interfaces *.h* )

FILE(GLOB_RECURSE h_SFML SFML/*.h*)
FILE(GLOB_RECURSE s_SFML SFML/*.c*)

source_group(Interfaces FILES ${interfaces})
source_group(Raster FILES ${h_SFML} ${s_SFML})

#---------------------------------------
# Project

add_library( SoundEngine STATIC ${s_SFML} ${h_SFML} ${interfaces} )
target_link_libraries(SoundEngine PlatformLibrary SupportLibrary ${EXC_LINK_LIBRARY_SFML})

add_subdirectory( SFML )