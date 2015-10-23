#-------------------------------------------------------------------------------
# PLATFORM LIBRARY
#
# Library for platform specific things like windowing, timers, etc.
#-------------------------------------------------------------------------------

message( "-Platform library" )

# enumerate source files
FILE( GLOB sources *.c* )
FILE( GLOB headers *.h* )

if(EXCESSIVE_WINDOWS)
	FILE(GLOB_RECURSE platform_sources Windows/*.c*)
	FILE(GLOB_RECURSE platform_headers Windows/*.h*)
elseif(EXCESSIVE_LINUX)
	FILE(GLOB_RECURSE platform_sources Linux/*.c*)
	FILE(GLOB_RECURSE platform_headers Linux/*.h*)
elseif(EXCESSIVE_OSX)
	FILE(GLOB_RECURSE platform_sources OSX/*.c*)
	FILE(GLOB_RECURSE platform_headers OSX/*.h*)
endif()

# add build target
add_library( PlatformLibrary STATIC ${sources} ${platform_sources} ${headers} ${platform_headers})