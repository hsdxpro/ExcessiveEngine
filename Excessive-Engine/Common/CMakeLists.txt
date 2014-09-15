FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_executable( TestCommon ${sources} )

target_link_libraries( TestCommon json_vc71_libmtd )