#-------------------------------------------------------------------------------
# GRAPICS ENGINE RASTER
#
# Rasterization based graphics engine implementation.
#-------------------------------------------------------------------------------

message( "-GraphicsEngineRaster" )

#---------------------------------------
# Files

FILE( GLOB_RECURSE sources *.c* )
FILE( GLOB_RECURSE headers *.h* )

#---------------------------------------
# Filters

# Global filter
SOURCE_GROUP(\\ FILES $(sources) $(headers))

# Main
SOURCE_GROUP(Main FILES GraphicsEngineRaster.h GraphicsEngineRaster.cpp)

# Utility
SOURCE_GROUP(Utility FILES utility/ref_ptr.h)

# Scene
SOURCE_GROUP(Scene FILES Scene.h Scene.cpp)

# Interface
SOURCE_GROUP(Interfaces FILES ${interfaces})

# Pipeline graph stuff
SOURCE_GROUP(PipelineGraph FILES 
PipelineData.cpp
PipelineData.h
PipelineNode.cpp
PipelineNode.h
PipelineNodeFactory.cpp
PipelineNodeFactory.h
PipelinePorts.cpp
PipelinePorts.h
)

# Resource management files filter
SOURCE_GROUP("Resources" FILES
Texture.h
Texture.cpp
Mesh.h
Mesh.cpp
Material.h
Material.cpp
ResourceManager.h
ResourceManager.cpp
)

# Scene management files filter
SOURCE_GROUP("Scene" FILES
SceneManager.h
SceneManager.cpp
Light.h
Light.cpp
Entity.h
Entity.cpp
Camera.h
Camera.cpp
)

#---------------------------------------
# Project

if(EXCESSIVE_DLL_MODULES)
	add_library( GraphicsEngineRaster SHARED ${sources} ${headers} ${interfaces} )
else()
	add_library( GraphicsEngineRaster STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries(GraphicsEngineRaster PlatformLibrary SupportLibrary)
target_link_libraries(GraphicsEngineRaster
	debug sfml-graphics-d 
	debug sfml-system-d 
	optimized sfml-graphics 
	optimized sfml-system)


