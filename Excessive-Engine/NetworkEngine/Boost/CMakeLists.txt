#-------------------------------------------------------------------------------
# NETWORK ENGINE BOOST
#
#-------------------------------------------------------------------------------

message( "-NetworkEngineBoost" )

#---------------------------------------
# Files

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( NetworkEngineBoost SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( NetworkEngineBoost STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries( NetworkEngineBoost PlatformLibrary SupportLibrary)
