#-------------------------------------------------------------------------------
# SUPPORT LIBRARY
#
# Platform independent support library.
# Contains common code that is widely used across the whole engine, i.e. math,
# memory allocators, stl extensions, data structures, algorithms etc.
#-------------------------------------------------------------------------------

message( "-Support library" )

# enumerate source files
FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

# add build target
add_library( SupportLibrary STATIC ${sources} ${headers} )

target_link_libraries(SupportLibrary 
assimp
)