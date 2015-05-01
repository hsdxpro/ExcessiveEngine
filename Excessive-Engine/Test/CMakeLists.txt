message( "-Test" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

include_directories("../Deployment/src/breakpad")
include_directories("../Common/src")
include_directories("../GraphicsEngine_Interface/interface")
add_executable( Test ${sources} ${headers} )
target_link_libraries( Test jsoncpp-d )
target_link_libraries( Test Common )
target_link_libraries( Test
exception_handlerd
crash_generation_serverd
crash_generation_clientd
commond )
target_link_libraries( Test debug physfs-d optimized physfs )
