#---------------------------------------------------------------
# BASE LIBRARY
#
# Hold the foundation of the whole engine. A common platform
# all modules can be built upon.
#---------------------------------------------------------------

message( "-Base Library" )

# sources

# type system
FILE( GLOB_RECURSE type_sources "TypeSystem_/*.c*" )
FILE( GLOB_RECURSE type_headers "TypeSystem_/*.h*" )

SOURCE_GROUP("TypeSystem" FILES ${type_sources} ${type_headers})

# serialization
FILE( GLOB_RECURSE serialization_sources "Serialization_/*.c*" )
FILE( GLOB_RECURSE serialization_headers "Serialization_/*.h*" )

SOURCE_GROUP("Serialization" FILES ${serialization_sources} ${serialization_headers})

# logging
FILE( GLOB_RECURSE logging_sources "Logging_/*.c*" )
FILE( GLOB_RECURSE logging_headers "Logging_/*.h*" )

SOURCE_GROUP("Logging" FILES ${logging_sources} ${logging_headers})

# utility
FILE( GLOB_RECURSE utility_sources "Utility_/*.c*" )
FILE( GLOB_RECURSE utility_headers "Utility_/*.h*" )

SOURCE_GROUP("Utility" FILES ${utility_sources} ${utility_headers})

# platform specific files
if(EXCESSIVE_WINDOWS)
	set(platform_name "PLATFORM_Windows")
elseif(EXCESSIVE_LINUX)
	set(platform_name "PLATFORM_Linux")
elseif(EXCESSIVE_OSX)
	set(platform_name "PLATFORM_OS X")
endif()


if (platform_name)
	FILE(GLOB_RECURSE platform_sources ${platform_name}/*.c*)
	FILE(GLOB_RECURSE platform_headers ${platform_name}/*.h*)
endif()

SOURCE_GROUP("${platform_name}/Sources" FILES ${platform_sources})
SOURCE_GROUP("${platform_name}/Headers" FILES ${platform_headers})

# add build target
add_library( BaseLibrary STATIC 
	${sources} ${headers}
	${type_sources} ${type_headers}
	${serialization_sources} ${serialization_headers}
	${utility_sources} ${utility_headers}
	${logging_sources} ${logging_headers}
	${platform_sources} ${platform_headers})

target_link_libraries(BaseLibrary ${EXC_LINK_LIBRARY_SFML})