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
FILE( GLOB_RECURSE interfaces "../PhysicsEngine/*.h*" )

#FILE( GLOB_RECURSE sources1 ../../../Externals/source/Bullet/*.c* )
#FILE( GLOB_RECURSE headers1 ../../../Externals/source/Bullet/*.h* )

SOURCE_GROUP(Math FILES ${sources} ${headers})
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Project
add_library( PhysicsEngine_Bullet STATIC ${sources} ${headers} ${sources1} ${headers1} ${interfaces})

# link stuff
target_link_libraries( PhysicsEngine_Bullet PlatformLibrary SupportLibrary ${EXC_LINK_LIBRARY_BULLET})