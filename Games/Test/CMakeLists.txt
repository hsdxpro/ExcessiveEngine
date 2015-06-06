message( "-Test" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

include_directories("../../Excessive-Engine/Deployment/src/breakpad")
include_directories("../../Excessive-Engine/Common/src")
include_directories("../../Excessive-Engine/GraphicsEngine_Interface/interface")
add_executable( Test ${sources} ${headers} )

if(DEBUG_BUILD)
  target_link_libraries( Test jsoncpp-d )
  target_link_libraries( Test
  exception_handlerd
  crash_generation_serverd
  crash_generation_clientd
  commond )
  target_link_libraries( Test physfs-d )
  target_link_libraries( Test sqlite-d )
else()
  target_link_libraries( Test jsoncpp )
  target_link_libraries( Test
  exception_handler
  crash_generation_server
  crash_generation_client
  common )
  target_link_libraries( Test physfs )
  target_link_libraries( Test sqlite )
endif()

target_link_libraries( Test Common )