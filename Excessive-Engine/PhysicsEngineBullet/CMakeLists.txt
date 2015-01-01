message( "-PhysicsEngineBullet" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../Module_Interfaces/PhysicsEngine/*)

include_directories("../Module_Interfaces/PhysicsEngine/")

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( PhysicsEngineBullet SHARED ${sources} ${headers} ${interfaces} )
	target_link_libraries( PhysicsEngineBullet "-Wl,--whole-archive" Common  "-Wl,--no-whole-archive")
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( PhysicsEngineBullet STATIC ${sources} ${headers} ${interfaces} )
endif()
