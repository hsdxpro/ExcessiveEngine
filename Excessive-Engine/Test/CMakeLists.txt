message( "-Test" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

include_directories("../Common/src")
include_directories("../GraphicsEngine_Interface/interface")
add_executable( Test ${sources} ${headers} )
target_link_libraries( Test json_vc71_libmtd )