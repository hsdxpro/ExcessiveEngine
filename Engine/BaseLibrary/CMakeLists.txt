#---------------------------------------------------------------
# BASE LIBRARY
#
# Hold the foundation of the whole engine. A common platform
# all modules can be built upon.
#---------------------------------------------------------------

message( "-Base Library" )

# enumerate source files
FILE( GLOB sources *.c* )
FILE( GLOB headers *.h* )

if(EXCESSIVE_WINDOWS)
	FILE(GLOB_RECURSE platform_sources Windows/*.c*)
	FILE(GLOB_RECURSE platform_headers Windows/*.h*)
	set(platform_name "Windows")
elseif(EXCESSIVE_LINUX)
	FILE(GLOB_RECURSE platform_sources Linux/*.c*)
	FILE(GLOB_RECURSE platform_headers Linux/*.h*)
	set(platform_name "Linux")
elseif(EXCESSIVE_OSX)
	FILE(GLOB_RECURSE platform_sources OSX/*.c*)
	FILE(GLOB_RECURSE platform_headers OSX/*.h*)
	set(platform_name "OS X")
endif()


SOURCE_GROUP("${platform_name}/Sources" FILES ${platform_sources})
SOURCE_GROUP("${platform_name}/Headers" FILES ${platform_headers})


# add build target
add_library( BaseLibrary STATIC ${sources} ${platform_sources} ${headers} ${platform_headers})

target_link_libraries(BaseLibrary ${EXC_LINK_LIBRARY_SFML})