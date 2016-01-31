#-------------------------------------------------------------------------------
# GRAPICS API
#
# Wrapper for the graphics api's interface.
#-------------------------------------------------------------------------------

# enumerate interfaces
FILE(GLOB interfaces *.h*)

FILE(GLOB_RECURSE h_DX11 DX11/*.c*)
FILE(GLOB_RECURSE s_DX11 DX11/*.h*)

FILE(GLOB_RECURSE h_OpenGL OpenGL/*.h*)
FILE(GLOB_RECURSE s_OpenGL OpenGL/*.c*)

FILE(GLOB_RECURSE h_VULKAN VULKAN/*.h*)
FILE(GLOB_RECURSE s_VULKAN VULKAN/*.c*)

source_group(Interfaces FILES ${interfaces})
source_group(DX11 FILES ${h_DX11} ${s_DX11})
source_group(OpenGL FILES ${h_OpenGL} ${s_OpenGL})
source_group(VULKAN FILES ${h_VULKAN} ${s_VULKAN})

# add build target
add_library( Gapi STATIC 
							${h_DX11} ${s_DX11} 
							${h_OpenGL} ${s_OpenGL} 
							${h_VULKAN} ${s_VULKAN} 
							${interfaces} 
			)

# linker input
target_link_libraries( Gapi PlatformLibrary SupportLibrary ${EXC_LINK_LIBRARY_SFML})

target_link_libraries( Gapi ${EXC_LINK_LIBRARY_GLEW} opengl32 )