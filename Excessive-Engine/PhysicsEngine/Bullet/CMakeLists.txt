message( "-PhysicsEngineBullet" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Interfaces/*)

include_directories("../../../Externals/include/Bullet3")

SOURCE_GROUP( Interfaces FILES ${interfaces})

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( PhysicsEngineBullet SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( PhysicsEngineBullet "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( PhysicsEngineBullet STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries( PhysicsEngineBullet 	Bullet3_d/Bullet3Collision_Debug 
						Bullet3_d/Bullet3Common_Debug
						Bullet3_d/Bullet3Dynamics_Debug
						Bullet3_d/Bullet3Geometry_Debug
						Bullet3_d/Bullet3OpenCL_clew_Debug
						Bullet3_d/BulletCollision_Debug
						Bullet3_d/BulletDynamics_Debug
						Bullet3_d/BulletSoftBody_Debug
						Bullet3_d/LinearMath_Debug )