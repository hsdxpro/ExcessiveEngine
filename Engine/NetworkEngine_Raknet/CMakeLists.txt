#-------------------------------------------------------------------------------
# NETWORK ENGINE
#
#-------------------------------------------------------------------------------

FILE( GLOB_RECURSE interfaces "../NetworkEngine/*.h*")
FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_library( NetworkEngine_Raknet STATIC ${sources} ${headers} ${interfaces} )

target_link_libraries( NetworkEngine_Raknet PlatformLibrary SupportLibrary)