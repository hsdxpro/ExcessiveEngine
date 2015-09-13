#-------------------------------------------------------------------------------
# EXCESSIVE-STRIKE
#
# One word to describe: FABULOUS
#-------------------------------------------------------------------------------

message( "-Excessive-Strike" )

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

add_executable( Excessive-Strike ${sources} ${headers} )

target_link_libraries( Excessive-Strike Core )
