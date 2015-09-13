#-------------------------------------------------------------------------------
# PHYSICS ENGINE BULLET
#
# Physics engine is responsible for all game physics simulation. May have multiple
# implementations.
#-------------------------------------------------------------------------------

message( "-PhysicsEngineBullet" )

#---------------------------------------
# Files

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

FILE( GLOB_RECURSE sources1 ../../../Externals/source/Bullet3/*.c* )
FILE( GLOB_RECURSE headers1 ../../../Externals/source/Bullet3/*.h* )

include_directories("../../../Externals/source/")
include_directories("../../../Externals/source/Bullet3")

# Project
if(EXCESSIVE_DLL_MODULES)
	add_library( PhysicsEngineBullet SHARED ${sources} ${headers} ${sources1} ${headers1} )
else()
	add_library( PhysicsEngineBullet STATIC ${sources} ${headers} ${sources1} ${headers1} )
endif()

# link stuff
target_link_libraries( PhysicsEngineBullet PlatformLibrary SupportLibrary)