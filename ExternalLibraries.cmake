#-------------------------------------------------------------------------------
# 
#-------------------------------------------------------------------------------

#---------------------------------------
# specify include directories
#---------------------------------------
include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/source)
include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/source/Bullet)

#---------------------------------------
# get platform dependent library collection
#---------------------------------------

# get platform
if (EXCESSIVE_WINDOWS)
	set(EXCESSIVE_LINK_OS "win")
elseif (EXCESSIVE_LINUX)
	set(EXCESSIVE_LINK_OS "linux")
elseif (EXCESSIVE_OSX)
	set(EXCESSIVE_LINK_OS "osx")
else ()
	set(EXCESSIVE_LINK_OS "unknown")
endif () 

# get compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(EXCESSIVE_LINK_COMPILER "gcc")
elseif ("MSVC" STREQUAL "${CMAKE_CXX_COMPILER_ID}")
	if (MSVC12)
		set(EXCESSIVE_LINK_COMPILER "msvc12")
	elseif (MSVC14)
		set(EXCESSIVE_LINK_COMPILER "msvc14")
	endif ()
else ()
	set(EXCESSIVE_LINK_COMPILER "unknown")
endif ()

# you can not compile with msvc12 any longer
if ("msvc12" STREQUAL ${EXCESSIVE_LINK_COMPILER})
	message(FATAL_ERROR "Microsoft Visual C++ 2013 is not supported. Use MSVC 2015 or later.")
endif()

# get architecture
if (EXCESSIVE_64BIT)
	set(EXCESSIVE_LINK_ARCHITECTURE "64")
elseif (EXCESSIVE_32BIT)
	set(EXCESSIVE_LINK_ARCHITECTURE "32")
else ()
	set(EXCESSIVE_LINK_ARCHITECTURE "unknown")
endif ()

# display information 
set(EXCESSIVE_LINK_PLATFORM "${EXCESSIVE_LINK_OS}_${EXCESSIVE_LINK_COMPILER}_${EXCESSIVE_LINK_ARCHITECTURE}")
message("Looking for externals in: ${EXCESSIVE_LINK_PLATFORM}")

# set up externals path
if ((${EXCESSIVE_LINK_PLATFORM} MATCHES ".*unknown.*") OR NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Externals/lib/${EXCESSIVE_LINK_PLATFORM})
	set(EXCESSIVE_LINK_PATH custom)
else ()
	set(EXCESSIVE_LINK_PATH ${CMAKE_CURRENT_LIST_DIR}/Externals/lib/${EXCESSIVE_LINK_PLATFORM})
endif ()

link_directories(${EXCESSIVE_LINK_PATH})

message("External link path: ${EXCESSIVE_LINK_PATH}")


#---------------------------------------
# add quick entries for common libraries
#---------------------------------------

add_definitions(-DSFML_STATIC)

set(EXC_LINK_LIBRARY_SFML
	winmm
	gdi32
	opengl32

	freetype
	openal32
	vorbis
	vorbisenc
	vorbisfile
	
	debug ogg-d
	optimized ogg
	debug flac-d
	optimized flac
	debug jpeg-d 
	optimized jpeg
	
	debug sfml-audio-s-d
	debug sfml-graphics-s-d
	debug sfml-system-s-d
	debug sfml-window-s-d
	debug sfml-network-s-d
	optimized sfml-audio-s	
	optimized sfml-graphics-s	
	optimized sfml-system-s
	optimized sfml-window-s
	optimized sfml-network-s
)


set(EXC_LINK_LIBRARY_ASSIMP
	debug assimp-d
	optimized assimp
)


add_definitions(-DBT_USE_SSE_IN_API)

set(EXC_LINK_LIBRARY_BULLET
	debug Bullet2FileLoader-d
	debug Bullet3Collision-d
	debug Bullet3Common-d
	debug Bullet3Dynamics-d
	debug Bullet3Geometry-d
	debug Bullet3OpenCL_clew-d
	debug BulletCollision-d
	debug BulletDynamics-d
	debug BulletSoftBody-d	
	
	optimized Bullet2FileLoader
	optimized Bullet3Collision
	optimized Bullet3Common
	optimized Bullet3Dynamics
	optimized Bullet3Geometry
	optimized Bullet3OpenCL_clew
	optimized BulletCollision
	optimized BulletDynamics
	optimized BulletSoftBody
	
	debug LinearMath-d 
	optimized LinearMath
)


add_definitions(-DGLEW_STATIC)

set(EXC_LINK_LIBRARY_GLEW
	debug glew-d
	optimized glew
)
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	