message( "-GraphicsEngineRaster" )

#########################################################################
# Files

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../GraphicsEngine_Interface/interface/*)

#########################################################################
# Filters

# Global filter
SOURCE_GROUP(\\ FILES $(sources) $(headers))

# Main
SOURCE_GROUP(Main FILES src/GraphicsEngine.h src/GraphicsEngine.cpp)

# Utility
SOURCE_GROUP(Utility FILES src/utility/ref_ptr.h)

# Scene
SOURCE_GROUP(Scene FILES src/Scene.h src/Scene.cpp)

# Interface
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Resource management files filter
SOURCE_GROUP("Resources" FILES
src/Texture.h
src/Texture.cpp
src/Mesh.h
src/Mesh.cpp
src/Material.h
src/Material.cpp
src/ResourceManager.h
src/ResourceManager.cpp
src/ResourceLoader.h
src/ResourceLoader.cpp
)

#Scene management files filter
SOURCE_GROUP("Scene" FILES
src/SceneManager.h
src/SceneManager.cpp
src/Light.h
src/Light.cpp
src/Entity.h
src/Entity.cpp
)

#########################################################################
# Project

include_directories("../GraphicsApi_Interface/interface/")

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsEngineRaster SHARED ${sources} ${headers} ${interfaces} )
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsEngineRaster STATIC ${sources} ${headers} ${interfaces} )
endif()
