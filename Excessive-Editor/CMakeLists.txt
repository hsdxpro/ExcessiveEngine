#-------------------------------------------------------------------------------
# EDITOR
#
# Ricsi's super awesome level editor program
#-------------------------------------------------------------------------------

message( "-Editor" )

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_executable( Editor ${sources} ${headers} )

target_link_libraries( Editor PlatformLibrary SupportLibrary )

target_link_libraries( Editor debug Awesomium_1_7_5/awesomium)
target_link_libraries( Editor optimized Awesomium_1_7_5/awesomium) 
