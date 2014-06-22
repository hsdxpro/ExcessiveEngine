cmake_minimum_required( VERSION 2.6 )

set( project_name Unnamed-Team )
set( DEBUG_BUILD true )
set( ARCH_32BIT false )
set( CMAKE_VERBOSE_MAKEFILE ON )

project( ${project_name} )

#set platform detection etc.
include( Unnamed-Engine/PIL/platform_detection.cmake )

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
add_subdirectory( Unnamed-Engine )

# Editor
add_subdirectory( Unnamed-Editor )

# Game
add_subdirectory( Games )

# tools
add_subdirectory( Tools )
