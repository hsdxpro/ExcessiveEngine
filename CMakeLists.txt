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
set(EXC_PROJECT_NAME "Excessive-Team")
project(${EXC_PROJECT_NAME})

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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_PERFDEBUG} -DEXC_DEBUG")
set(CMAKE_CXX_FLAGS_PERFDEBUG "${CMAKE_CXX_FLAGS_PERFDEBUG} -DEXC_PERFDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_PERFDEBUG} -DEXC_RELEASE")

set(DEBUG_CONFIGURATIONS Debug;PerfDebug)

# make modules as DLLs
option(EXC_DYNAMIC_LINK_MODULES "Chooses whether to create static libs for the submodules, or make them as DLLs." ON)
if (EXC_DYNAMIC_LINK_MODULES)
	add_definitions(-DEXC_DYNAMIC_LINK_MODULES)
endif()

#---------------------------------------
# platform detection
#---------------------------------------

# operating system
if(WIN32)
	set(EXC_WINDOWS 1)
	message("Operating system is: Windows")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(EXC_LINUX 1)
	message("Operating system is: Linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(EXC_OSX 1)
	message("Operating system is: OSX")
endif()

# 32 or 64 bit
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(EXC_64BIT 1)
	message("Architecture is 64 bits")
else ()
	set(EXC_32BIT 1)
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
if (EXC_64BIT)
	add_definitions(-DEXC_64BIT)
else ()
	add_definitions(-DEXC_32BIT)
endif()

if(EXC_WINDOWS)
	add_definitions(-DEXC_WINDOWS)
elseif(EXC_LINUX)
	add_definitions(-DEXC_LINUX)
elseif(EXC_OSX)
	add_definitions(-DEXC_OSX)
endif()


#---------------------------------------
# add external libraries
#---------------------------------------

include_directories(${CMAKE_CURRENT_LIST_DIR}/Externals/include)

# get platform
if (EXC_WINDOWS)
	set(EXC_LINK_OS "win")
elseif (EXC_LINUX)
	set(EXC_LINK_OS "linux")
elseif (EXC_OSX)
	set(EXC_LINK_OS "osx")
else ()
	set(EXC_LINK_OS "unknown")
endif () 

# get compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(EXC_LINK_COMPILER "gcc")
elseif ("MSVC" STREQUAL "${CMAKE_CXX_COMPILER_ID}")
	if (MSVC12)
		set(EXC_LINK_COMPILER "msvc12")
	elseif (MSVC14)
		set(EXC_LINK_COMPILER "msvc14")
	endif ()
else ()
	set(EXC_LINK_COMPILER "unknown")
endif ()

# get architecture
if (EXC_64BIT)
	set(EXC_LINK_ARCHITECTURE "64")
elseif (EXC_32BIT)
	set(EXC_LINK_ARCHITECTURE "32")
else ()
	set(EXC_LINK_ARCHITECTURE "unknown")
endif ()

# display information 
set(EXC_LINK_PLATFORM "${EXC_LINK_OS}_${EXC_LINK_COMPILER}_${EXC_LINK_ARCHITECTURE}")
message("Looking for externals in: ${EXC_LINK_PLATFORM}")

# set up externals path
if ((${EXC_LINK_PLATFORM} MATCHES ".*unknown.*") OR NOT IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Externals/lib/${EXC_LINK_PLATFORM})
	set(EXC_LINK_PATH custom)
else ()
	set(EXC_LINK_PATH ${CMAKE_CURRENT_LIST_DIR}/Externals/lib/${EXC_LINK_PLATFORM})
endif ()

link_directories(${EXC_LINK_PATH})

message("External link path: ${EXC_LINK_PATH}")


#---------------------------------------
# add sub-folders
#---------------------------------------

# Engine
add_subdirectory( Excessive-Engine )
# Editor
add_subdirectory( Excessive-Editor )
# Game
add_subdirectory( Games )











