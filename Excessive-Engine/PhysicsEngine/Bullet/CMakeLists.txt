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

include_directories("../../../Externals/include/Bullet3/")

SOURCE_GROUP( Interfaces FILES ${interfaces})


# Project
if(EXC_DYNAMIC_LINK_MODULES)
	add_library( PhysicsEngineBullet SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( PhysicsEngineBullet STATIC ${sources} ${headers} ${interfaces} )
endif()

# link stuff anyway
target_link_libraries( PhysicsEngineBullet PlatformLibrary SupportLibrary)
# link stuff in debug builds
target_link_libraries( PhysicsEngineBullet 	
	debug Bullet3_d/Bullet3Collision_Debug 
	debug Bullet3_d/Bullet3Common_Debug
	debug Bullet3_d/Bullet3Dynamics_Debug
	debug Bullet3_d/Bullet3Geometry_Debug
	debug Bullet3_d/Bullet3OpenCL_clew_Debug
	debug Bullet3_d/BulletCollision_Debug
	debug Bullet3_d/BulletDynamics_Debug
	debug Bullet3_d/BulletSoftBody_Debug
	debug Bullet3_d/LinearMath_Debug )
#link stuff in release builds
target_link_libraries( PhysicsEngineBullet 
	optimized Bullet3_r/Bullet3Collision 
	optimized Bullet3_r/Bullet3Common
	optimized Bullet3_r/Bullet3Dynamics
	optimized Bullet3_r/Bullet3Geometry
	optimized Bullet3_r/Bullet3OpenCL_clew
	optimized Bullet3_r/BulletCollision
	optimized Bullet3_r/BulletDynamics
	optimized Bullet3_r/BulletSoftBody
	optimized Bullet3_r/LinearMath )