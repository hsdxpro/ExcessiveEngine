message( "-root" )

cmake_minimum_required( VERSION 2.6 )

set( project_name Excessive-Team )
set( ARCH_32BIT true )
set( CMAKE_VERBOSE_MAKEFILE ON )

OPTION(BUILD_EXCESSIVE_WITH_DLL "BUILD_EXCESSIVE_WITH_DLL" ON)
OPTION(DEBUG_BUILD "DEBUG_BUILD" ON)


if(BUILD_EXCESSIVE_WITH_DLL)
	add_definitions( -DBUILD_DLL )
else()
	add_definitions( -DBUILD_STATIC )
endif()

if(WIN32)
	add_definitions( -DWIN )
elseif(UNIX)
	add_definitions( -DLIN )
elseif(APPLE)
	add_definitions( -DMAC )
endif()

if(DEBUG_BUILD)
	set(BUILD_CONFIG_LIST_DEBUG 	D_32) #D_64
else()
	set(BUILD_CONFIG_LIST_RELEASE 	R_32) #R_64
endif()

foreach(buildName ${BUILD_CONFIG_LIST_DEBUG})
	string(TOUPPER ${buildName} buildName)

	set(CMAKE_CXX_FLAGS_${buildName} ${CMAKE_CXX_FLAGS_DEBUG})
	set(CMAKE_EXE_LINKER_FLAGS_${buildName} ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
	set(CMAKE_MODULE_LINKER_FLAGS_${buildName} ${CMAKE_MODULE_LINKER_FLAGS_DEBUG})
	set(CMAKE_SHARED_LINKER_FLAGS_${buildName} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
endforeach()

foreach(buildName ${BUILD_CONFIG_LIST_RELEASE})
	string(TOUPPER ${buildName} buildName)

	set(CMAKE_CXX_FLAGS_${buildName} ${CMAKE_CXX_FLAGS_RELEASE})
	set(CMAKE_EXE_LINKER_FLAGS_${buildName} ${CMAKE_EXE_LINKER_FLAGS_RELEASE} )
	set(CMAKE_MODULE_LINKER_FLAGS_${buildName} ${CMAKE_MODULE_LINKER_FLAGS_RELEASE})
	set(CMAKE_SHARED_LINKER_FLAGS_${buildName} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})
endforeach()

# Solution Configurations
if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES ${BUILD_CONFIG_LIST_DEBUG} ${BUILD_CONFIG_LIST_RELEASE} )
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
endif()

project( ${project_name} )

#set platform detection etc.
include( Excessive-Engine/PIL/platform_detection.cmake )

if( WIN_BUILD )
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../ )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../ )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../ )
else()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
endif()

#add externals
include( Externals/externals.cmake )

# Engine
add_subdirectory( Excessive-Engine )

# Editor
add_subdirectory( Excessive-Editor )

# Game
add_subdirectory( Games )

# tools
add_subdirectory( Tools )
