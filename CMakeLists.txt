#-------------------------------------------------------------------------------
# ROOT
#
# This file is the root CMake file. 
# Contains basic stuff like platform detection, global preprocessor definitions,
# build configurations.
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

# add flags for perf debug, others should have it right
set(CMAKE_CXX_FLAGS_PERFDEBUG ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
set(CMAKE_EXE_LINKER_FLAGS_PERFDEBUG ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} )
set(CMAKE_MODULE_LINKER_FLAGS_PERFDEBUG ${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO})
set(CMAKE_SHARED_LINKER_FLAGS_PERFDEBUG ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO})

# add preprocessor definitions for configs
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DEXCESSIVE_DEBUG")
set(CMAKE_CXX_FLAGS_PERFDEBUG "${CMAKE_CXX_FLAGS_PERFDEBUG} -DEXCESSIVE_PERFDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DEXCESSIVE_RELEASE")

set(DEBUG_CONFIGURATIONS Debug;PerfDebug)

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
# directories
#---------------------------------------

# binary output directories
if (CMAKE_BUILD_TYPE)
	set(EXCESSIVE_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/Build/${CMAKE_BUILD_TYPE}")
else ()
	set(EXCESSIVE_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/Build")
endif ()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EXCESSIVE_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXCESSIVE_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXCESSIVE_OUTPUT_DIR})

# clean old stuff
message("Cleaning old output directory...")
file(REMOVE_RECURSE ${EXCESSIVE_OUTPUT_DIR})

# asset directory (temporary solution)
add_definitions(-DEXCESSIVE_ASSET_DIR="${CMAKE_CURRENT_LIST_DIR}/Assets/")


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
include(ExternalLibraries.cmake)


#---------------------------------------
# add sub-folders
#---------------------------------------

include_directories(${CMAKE_CURRENT_LIST_DIR}/Engine)

# Engine
add_subdirectory( Engine )
# Game
add_subdirectory( Games )
# Editor
#add_subdirectory( Editor )


#---------------------------------------
# copy runtime dll's and files
#---------------------------------------

message("Copying runtime dependencies to build folder...")
if (CMAKE_BUILD_TYPE)
	file(MAKE_DIRECTORY ${EXCESSIVE_OUTPUT_DIR})
	
	message("   Single config build system detected: ")

	if (";${DEBUG_CONFIGURATIONS};" MATCHES ";${CMAKE_BUILD_TYPE};")
		file(COPY "${EXCESSIVE_LINK_PATH}/runtime/debug/" DESTINATION "${EXCESSIVE_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}")
		message("      ${CMAKE_BUILD_TYPE} - type: DEBUG")
	else ()
		file(COPY "${EXCESSIVE_LINK_PATH}/runtime/release/" DESTINATION "${EXCESSIVE_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}")
		message("      ${CMAKE_BUILD_TYPE} - type: RELEASE")
	endif()
else ()
	message("   Multi-config build system detected, processing configs:")
	foreach (config ${CMAKE_CONFIGURATION_TYPES})
		file(MAKE_DIRECTORY "${EXCESSIVE_OUTPUT_DIR}/${config}")
	
		if (";${DEBUG_CONFIGURATIONS};" MATCHES ";${config};")
			file(COPY "${EXCESSIVE_LINK_PATH}/runtime/debug/" DESTINATION "${EXCESSIVE_OUTPUT_DIR}/${config}")
			message("      ${config} - type: DEBUG")
		else ()
			file(COPY "${EXCESSIVE_LINK_PATH}/runtime/release/" DESTINATION "${EXCESSIVE_OUTPUT_DIR}/${config}")
			message("      ${config} - type: RELEASE")
		endif()	
	endforeach()
endif ()











