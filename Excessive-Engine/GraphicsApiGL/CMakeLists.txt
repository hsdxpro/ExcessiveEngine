message( "-GraphicsApiGL" )

FILE( GLOB_RECURSE sources src/*.c* )
FILE( GLOB_RECURSE headers src/*.h* )
FILE( GLOB_RECURSE interfaces ../GraphicsApi_Interface/interface/*)

include_directories( "../GraphicsApi_Interface/interface/" )
include_directories( "../Common/src/" )

if(BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsApiGL SHARED ${sources} ${headers} ${interfaces} )
elseif( NOT BUILD_EXCESSIVE_WITH_DLL)
	add_library( GraphicsApiGL STATIC ${sources} ${headers} ${interfaces} )
endif()

target_link_libraries( GraphicsApiGL glew32 opengl32 )

add_executable( GLGapiTest test.cpp )
add_dependencies( GLGapiTest GraphicsApiGL )
target_link_libraries( GLGapiTest Common)