message( "-Editor" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )

add_executable( Editor ${sources} ${headers} )

target_link_libraries( Editor Common )

if(DEBUG_BUILD)
 target_link_libraries( Editor Awesomium_1_7_5/awesomium)
else()
 target_link_libraries( Editor Awesomium_1_7_5/awesomium)  
endif()
