#-------------------------------------------------------------------------------
# TEST
#
# Various smaller tests included in this project.
#-------------------------------------------------------------------------------

message( "-Test" )

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_executable( Test ${sources} ${headers} )

target_link_libraries( Test PlatformLibrary SupportLibrary Core BaseLibrary)