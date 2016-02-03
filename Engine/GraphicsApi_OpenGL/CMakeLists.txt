FILE(GLOB_RECURSE interfaces "../GraphicsApi/*.h*")
FILE(GLOB_RECURSE headers *.h*)
FILE(GLOB_RECURSE sources *.c*)


add_library( GraphicsApi_OpenGL STATIC
			${interfaces} 
			${headers} 
			${sources} 
			)

# linker input
target_link_libraries( GraphicsApi_OpenGL PlatformLibrary SupportLibrary ${EXC_LINK_LIBRARY_SFML})
target_link_libraries( GraphicsApi_OpenGL ${EXC_LINK_LIBRARY_GLEW} opengl32 )