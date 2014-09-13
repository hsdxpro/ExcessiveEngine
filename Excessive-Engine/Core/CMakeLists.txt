cmake_minimum_required (VERSION 2.6)

PROJECT(Core C CXX)

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_library( Core STATIC ${sources} ${headers} )
set_target_properties( Core PROPERTIES LINKER_LANGUAGE CXX )