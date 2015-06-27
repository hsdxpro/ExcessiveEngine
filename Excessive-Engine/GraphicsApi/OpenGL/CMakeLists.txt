#-------------------------------------------------------------------------------
# GRAPICS API GL
#
# Wrapper for the OpenGL API.
#-------------------------------------------------------------------------------

message( "-GraphicsApiGL" )

# enumerate sources
FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

# add build target
if(EXCESSIVE_DLL_MODULES)
	add_library( GraphicsApiGL SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( GraphicsApiGL STATIC ${sources} ${headers} ${interfaces} )
endif()

# linker input
target_link_libraries( GraphicsApiGL glew32 opengl32 )
target_link_libraries( GraphicsApiGL PlatformLibrary SupportLibrary )