#-------------------------------------------------------------------------------
# ROOT
#
# This file is the root CMake file. 
# Contains basic stuff like platform definition, global preprocessor definitions,
# configuration types.
#-------------------------------------------------------------------------------

message("-Root")

cmake_minimum_required(VERSION 3.2)

#---------------------------------------
# create project
#---------------------------------------
set(EXCESSIVE_PROJECT_NAME "Excessive-Team")
project(${EXCESSIVE_PROJECT_NAME})

#---------------------------------------
# set build configurations
#---------------------------------------

# build configs
set(CMAKE_CONFIGURATION_TYPES Debug PerfDebug Release CACHE TYPE INTERNAL FORCE )

# add flags for perf debug, other should have it right
set(CMAKE_CXX_FLAGS_PERFDEBUG ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
set(CMAKE_EXE_LINKER_FLAGS_PERFDEBUG ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} )
set(CMAKE_MODULE_LINKER_FLAGS_PERFDEBUG ${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO})
set(CMAKE_SHARED_LINKER_FLAGS_PERFDEBUG ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO})

# add preprocessor definitions for configs
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXCESSIVE_DEBUG")
set(CMAKE_CXX_FLAGS_PERFDEBUG "${CMAKE_CXX_FLAGS_PERFDEBUG} -DEXCESSIVE_PERFDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DEXCESSIVE_RELEASE")

set(DEBUG_CONFIGURATIONS Debug;PerfDebug)

# make modules as DLLs
option(EXCESSIVE_DLL_MODULES "Chooses whether to create static libs for the submodules, or make them as DLLs." ON)
if (EXCESSIVE_DLL_MODULES)
	add_definitions(-DEXCESSIVE_DLL_MODULES)
endif()

#---------------------------------------
# platform detection
#---------------------------------------

# operating system
if(WIN32)
	set(EXCESSIVE_WINDOWS 1)
	message("Operating system is: Windows")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(EXCESSIVE_LINUX 1)
	message("Operating system is: Linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(EXCESSIVE_OSX 1)
	message("Operating system is: OSX")
endif()

# 32 or 64 bit
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(EXCESSIVE_64BIT 1)
	message("Architecture is 64 bits")
else ()
	set(EXCESSIVE_32BIT 1)
	message("Architecture is 32 bits")
endif ()

#---------------------------------------
# output directories
#---------------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#---------------------------------------
# preprocessor definitions
#---------------------------------------
if (EXCESSIVE_64BIT)
	add_definitions(-DEXCESSIVE_64BIT)
else ()
	add_definitions(-DEXCESSIVE_32BIT)
endif()

if(EXCESSIVE_WINDOWS)
	add_definitions(-DEXCESSIVE_WINDOWS)
elseif(EXCESSIVE_LINUX)
	add_definitions(-DEXCESSIVE_LINUX)
elseif(EXCESSIVE_OSX)
	add_definitions(-DEXCESSIVE_OSX)
endif()


#---------------------------------------
# add external libraries
#---------------------------------------

include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/include)

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
# add sub-folders
#---------------------------------------

include_directories(${CMAKE_CURRENT_LIST_DIR}/Excessive-Engine)

# Engine
add_subdirectory( Excessive-Engine )
# Editor
add_subdirectory( Excessive-Editor )
# Game
add_subdirectory( Games )


#---------------------------------------
# copy runtime dll's and files
#---------------------------------------
# TODO











