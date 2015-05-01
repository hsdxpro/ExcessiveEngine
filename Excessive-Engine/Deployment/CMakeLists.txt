message( "-Deployment" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

include_directories("../Common/src")
add_executable( Deployment ${sources} ${headers} )
target_link_libraries( Deployment jsoncpp-d )
target_link_libraries( Deployment Common )
