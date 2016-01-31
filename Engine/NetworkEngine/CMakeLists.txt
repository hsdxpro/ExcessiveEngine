#-------------------------------------------------------------------------------
# NETWORK ENGINE
#
#-------------------------------------------------------------------------------

FILE(GLOB interfaces *.h*)

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_library( NetworkEngine STATIC ${sources} ${headers} ${interfaces} )

target_link_libraries( NetworkEngine PlatformLibrary SupportLibrary)