#-------------------------------------------------------------------------------
# 
#-------------------------------------------------------------------------------

#---------------------------------------
# specify include directories
#---------------------------------------
include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/include)


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
	message(FATAL_ERROR "This version of Microsoft Visual C++ is not supported. Use MSVC 2015 or later.")
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

set(LINK_SFML_LIBRARIES
	debug SFML/flac-d
	optimized SFML/flac
	SFML/freetype
	SFML/jpeg
	SFML/ogg
	SFML/openal32
	debug SFML/sfml-audio-s-d
	optimized SFML/sfml-audio-s
	debug SFML/sfml-graphics-s-d
	optimized SFML/sfml-graphics-s
	debug SFML/sfml-network-s-d
	optimized SFML/sfml-network-s
	debug SFML/sfml-system-s-d
	optimized SFML/sfml-system-s
	debug SFML/sfml-window-s-d
	optimized SFML/sfml-window-s
	SFML/vorbis
	SFML/vorbisenc
	SFML/vorbisfile)
	
if (EXCESSIVE_WINDOWS)
	set(LINK_SFML_LIBRARIES 
		${LINK_SFML_LIBRARIES}
		debug SFML/win_utf8_io_static-d
		optimized SFML/win_utf8_io_static
		Winmm)
endif()
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
