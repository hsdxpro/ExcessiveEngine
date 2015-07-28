#-------------------------------------------------------------------------------
# TEST
#
# Various smaller tests included in this project.
#-------------------------------------------------------------------------------

message( "-Test" )

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

include_directories("../../Externals/include/Bullet3/")

add_executable( Test ${sources} ${headers} )

target_link_libraries( Test debug jsoncpp-d )
target_link_libraries( Test debug physfs-d )
target_link_libraries( Test debug sqlite-d )  
target_link_libraries( Test optimized jsoncpp )
target_link_libraries( Test optimized physfs )
target_link_libraries( Test optimized sqlite )

target_link_libraries( Test exception_handler crash_generation_server crash_generation_client common )

target_link_libraries( Test PlatformLibrary SupportLibrary Core)